#ifndef INC_META_H_
#define INC_META_H_

// Template metaprogramming utilities, taken from:
// https://gitlab.lisha.ufsc.br/epos/epos/-/blob/develop/include/system/meta.h

template<typename T1, typename T2>
struct EQUAL
{ enum { Result = false }; };

template<typename T>
struct EQUAL<T, T>
{ enum { Result = true }; };

#endif /* INC_META_H_ */
