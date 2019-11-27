#ifndef SX_TYPES_H
#define SX_TYPES_H

#define STL_TYPEDEF_EX(std_type, type) \
	typedef std_type type; \
	typedef std_type::iterator type##_##i; \
	typedef std_type::const_iterator type##_##ci; \
	typedef std_type::reverse_iterator type##_##ri; \
	typedef std_type::allocator_type type##_##al; \
	typedef std_type::size_type type##_##sz; \
	typedef std_type::difference_type type##_##dif; \
	typedef std_type::reference type##_##ref; \
	typedef std_type::const_reference type##_##cref; \
	typedef std_type::value_type type##_##v;

#define STL_TYPENAME_EX(std_type, type) \
	typedef std_type type; \
	typedef typename std_type::iterator type##_##i; \
	typedef typename std_type::const_iterator type##_##ci; \
	typedef typename std_type::reverse_iterator type##_##ri; \
	typedef typename std_type::allocator_type type##_##al; \
	typedef typename std_type::size_type type##_##sz; \
	typedef typename std_type::difference_type type##_##dif; \
	typedef typename std_type::reference type##_##ref; \
	typedef typename std_type::const_reference type##_##cref; \
	typedef typename std_type::value_type type##_##v;

#endif

