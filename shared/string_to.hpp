#ifndef GRAEHL__SHARED__STRING_TO_HPP
#define GRAEHL__SHARED__STRING_TO_HPP

#include <string>
#include <sstream>

namespace graehl {

template <class I,class To>
bool try_stream_into(I & i,To &to,bool complete=true)
{
    i >> to;
    if (i.fail()) return false;
    if (complete) {
        char c;
        return !(i >> c);
    }
    return true;
}

template <class Str,class To>
bool try_string_into(Str const& str,To &to,bool complete=true)
{
    std::istringstream i(str);
    return try_stream_into(i,to,complete);
}



template <class Str,class Data> inline
Data & string_into(const Str &str,Data &data)
{
    if (!try_string_into(str,data))
        throw std::runtime_error(std::string("Couldn't convert (string_into): ")+str);
    return data;
}


template <class Data,class Str> inline
Data string_to(const Str &str)
{
    Data ret;
    string_into(str,ret);
    return ret;
}

template <class D> inline
std::string to_string(D const &d)
{
    std::ostringstream o;
    o << d;
    return o.str();
}

template <class Str>
bool try_string_into(Str const& str,Str &to,bool complete=true)
{
    str=to;
    return true;
}


inline
std::string const& to_string(std::string const& d)
{
    return d;
}

template <class Str>
Str const& string_to(Str const &s)
{
    return s;
}

template <class Str>
Str & string_into(Str const &s,Str &d)
{
    return d=s;
}


/*

template <class Str,class Data,class size_type> inline
void substring_into(const Str &str,size_type pos,size_type n,Data &data)
{
//    std::istringstream i(str,pos,n); // doesn't exist!
    std::istringstream i(str.substr(pos,n));
    if (!(i>>*data))
        throw std::runtime_error("Couldn't convert (string_into): "+str);
}

template <class Data,class Str,class size_type> inline
Data string_to(const Str &str,size_type pos,size_type n)
{
    Data ret;
    substring_into(str,pos,n,ret);
    return ret;
}

*/

}


#endif
