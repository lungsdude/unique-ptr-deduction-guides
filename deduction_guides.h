#include <memory>
#include <tuple>
#include <type_traits>

namespace std {
	namespace detail {
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

	template <typename Ret, typename T, typename D = auto (*)(T*)->Ret>
	unique_ptr(nullptr_t, auto (*)(T*)->Ret) -> unique_ptr<T, D>;

	template <typename Obj, typename Ret, typename T, typename D = auto (Obj::*)(T*)->Ret>
	unique_ptr(nullptr_t, auto (Obj::*)(T*)->Ret) -> unique_ptr<T, D>;

	template <typename Obj, typename Ret, typename T, typename D = auto (Obj::*)(T*) const->Ret>
	unique_ptr(nullptr_t, auto (Obj::*)(T*) const->Ret) -> unique_ptr<T, D>;

	template <detail::functor Func>
	unique_ptr(nullptr_t, Func&&) -> unique_ptr<remove_pointer_t<tuple_element_t<0, detail::function_signature_t<Func>>>, Func>;

} // namespace std
