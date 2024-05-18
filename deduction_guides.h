namespace std {
	namespace detail {
		template <class D, typename = typename D::pointer>
		constexpr bool pointer_is_defined_impl(int) {
			return true;
		}

		template <class D>
		constexpr bool pointer_is_defined_impl(...) {
			return false;
		}

		template <class D>
		constexpr bool pointer_is_defined = pointer_is_defined_impl<D>(0);

		template <class T, class D, bool = pointer_is_defined<D>>
		struct pointer_is_same;

		template <class T, class D>
		struct pointer_is_same<T, D, false> : std::true_type {};

		template <class T, class D>
		struct pointer_is_same<T, D, true> : std::is_same<T*, typename D::pointer> {};

				template <typename Functor>
		concept functor = is_class_v<decay_t<Functor>> and requires(Functor&& func) { &decay_t<Functor>::operator(); };

		template <typename Sign>
		struct function_signature;

		template <typename Ret, typename... Args>
		struct function_signature<Ret(Args...)> {
			using type = std::tuple<Args...>;
			using ret = Ret;
			using obj = void;
		};

		template <typename Ret, typename Obj, typename... Args>
		struct function_signature<Ret (Obj::*)(Args...)> {
			using type = typename std::tuple<Args...>;
			using ret = Ret;
			using obj = Obj;
		};

		template <typename Ret, typename Obj, typename... Args>
		struct function_signature<Ret (Obj::*)(Args...) const> {
			using type = typename std::tuple<Args...>;
			using ret = Ret;
			using obj = Obj;
		};

		template <functor Functor>
		struct function_signature<Functor> {
		private:
			using temp = function_signature<decltype(&std::decay_t<Functor>::operator())>;

		public:
			using type = typename temp::type;
			using ret = temp::ret;
			using obj = temp::obj;
		};

		template <typename T>
		using function_signature_t = typename function_signature<T>::type;
	} // namespace detail

	// new customization point
	template <class>
	constexpr bool deleter_applicable_to_array = false;

	template <class T>
	constexpr bool deleter_applicable_to_array<default_delete<T[]>> = true;

	// [unique.ptr.single.ctor] p9
	template <class T, class D, typename = enable_if_t<is_object_v<D> && detail::pointer_is_same<T, D>::value && !deleter_applicable_to_array<D>>>
	unique_ptr(T*, D const&) -> unique_ptr<T, D>;

	template <class T, class D, typename = enable_if_t<is_object_v<D> && detail::pointer_is_same<T, D>::value && !deleter_applicable_to_array<D>>>
	unique_ptr(T*, D&&) -> unique_ptr<T, D>;

	// [unique.ptr.single.ctor] p10
	template <class T, class D, typename = enable_if_t<is_function_v<D>>>
	unique_ptr(T*, D&) -> unique_ptr<T, D&>;

	// [unique.ptr.runtime.ctor] p2
	template <class T, class D, typename = enable_if_t<is_object_v<D> && detail::pointer_is_same<T, D>::value && deleter_applicable_to_array<D>>>
	unique_ptr(T*, D const&) -> unique_ptr<T[], D>;

	template <class T, class D, typename = enable_if_t<is_object_v<D> && detail::pointer_is_same<T, D>::value && deleter_applicable_to_array<D>>>
	unique_ptr(T*, D&&) -> unique_ptr<T[], D>;

	////////////////
	// [unique.ptr.single.ctor] p9
	template <typename Ret, typename T, typename D = auto (*)(T*)->Ret>
	unique_ptr(nullptr_t, auto (*)(T*)->Ret) -> unique_ptr<T, D>;

	template <typename Obj, typename Ret, typename T, typename D = auto (Obj::*)(T*)->Ret>
	unique_ptr(nullptr_t, auto (Obj::*)(T*)->Ret) -> unique_ptr<T, D>;

	template <typename Obj, typename Ret, typename T, typename D = auto (Obj::*)(T*) const->Ret>
	unique_ptr(nullptr_t, auto (Obj::*)(T*) const->Ret) -> unique_ptr<T, D>;

	template <detail::functor Func>
	unique_ptr(nullptr_t, Func&&) -> unique_ptr<remove_pointer_t<tuple_element_t<0, detail::function_signature_t<Func>>>, Func>;
} // namespace std
