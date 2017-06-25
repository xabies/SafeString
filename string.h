#include <iostream>
#include <cstring>


namespace NUtil
{


    class XString: public std::exception
    {
        const char * what() const throw() 
        {
            return "XString Exception.\n"; 
        }
    };


    template <int ASIZE>
    char * itoa(int num, char (& str)[ASIZE], int base)
    {
        int i = 0;
        bool is_negative = false;

        //! Handle 0 case explicitely
        if (num == 0)
        {
            str[i++] = '0';
            str[i] = '\0';
            return str;
        }

        //! Negative numbers are handled only with base 10. Otherwise numbers are considered unsigned.
        if (num < 0 && base == 10)
        {
            is_negative = true;
            num = - num;
        }

        //! Process individual digits
        while (num != 0)
        {
            int rem = num % base;
            if (rem > 9)
            {
                str[i++] = (rem - 10) + 'a';
            }
            else
            {
                str[i++] = rem + '0';                    
            }
            num = num / base;
        }

        //! If number is negative, append '-'
        if (is_negative)
        {
            str[i++] = '-';
        }
        str[i] = '\0';

        //! Reverse the string
        char str_aux[ASIZE];
        int str_index = 0;
        for (int j = (i - 1); j >= 0; --j)
        {
            str_aux[str_index] = str[j];
            ++str_index;
        }
        str_aux[str_index] = '\0';

        return strncpy(str, str_aux, i);
    }


    template <int N>
    class CString 
    {


    public:


        CString() : m_size(0)
        {
            m_array[0] = '\0';
        }


        template <int ASIZE>
        CString(char const (& ach_array)[ASIZE])
        {
            add(ach_array);
        }

        // all the special members can be defaulted

        CString(CString const &) = default;

        CString(CString &&) = default;

        CString& operator=(CString const &) = default;

        CString& operator=(CString &&) = default;

        ~CString() = default;


        friend std::ostream & operator <<(std::ostream & OS, const CString & S)
        {
            OS << S.m_array;
            return OS;
        }


        friend std::istream & operator >>(std::istream & IS, CString & S)
        {
            IS >> S.array;
            return IS;
        }


        template <int ASIZE>
        friend CString & operator <<(CString & string, char const (& ach_array)[ASIZE])
        {
            string.add(ach_array);
            return string;
        }


        friend CString& operator <<(CString & string, const int num)
        {
	        string.add(num);
            return string;
        }


        void add(char c)
        {
            if ((m_size + 1) >= N)
            {
                throw XString();
            }

            m_array[m_size] = c;
            m_array[m_size + 1] = '\0';
            m_size++;
        }

        
        template <int ASIZE>
        void add(char const (& ach_array)[ASIZE])
        {
            int size_str = strnlen(ach_array, 256);
            if ((m_size + size_str) >= N)
            {
                throw XString();
            }

            strncpy(m_array + m_size, ach_array, (m_size + size_str));
            m_size = m_size + size_str;
            m_array[m_size] = '\0';
        }


        void add(int num)
        {
            char a64ch_str[64];
            itoa(num, a64ch_str, 10);
            add(a64ch_str);
        }


        char at(int position)
        {
            return m_array[position];
        }


        template <int ASIZE>
        char * get(char (& ach_array)[ASIZE])
        {
            return strncpy(ach_array, m_array, ASIZE);
        }


    private:

        char m_array[N + 1];

        int m_size;
        
    };


}
