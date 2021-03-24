/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/*****************************************************************************
* User of this library should read the documentation
* in the messaging.h file.
******************************************************************************/


#include "RichConsoleStyleMessageFormatter.h"

#include <sofa/helper/logging/Message.h>

#include <sofa/core/objectmodel/Base.h>
using sofa::helper::logging::SofaComponentInfo;

#include <sofa/helper/system/console.h>
#include <sofa/helper/fixed_array.h>

#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <boost/token_iterator.hpp>

#ifdef WIN32
#undef min
#undef max
#endif

#include <algorithm>

namespace { // Anonymous namespace

// Adapted from boost:char_separator
class char_separator;

// Adapted from boost:tokenizer
class tokenizer ; // Defined at the end of this file

}

namespace sofa::helper::logging
{

/////////////////////////////// STATIC ELEMENT SPECIFIC TO RichConsoleStyleMessage /////////////////
///
/// \brief simpleFormat a text containing our markdown 'tags'
/// \param jsize size of the line prefix to fill with space (for left side alignment)
/// \param text  the text to format
/// \param line_length number of column to render to to
/// \param wrapped the destination stream where to write the formatted text.
///
void simpleFormat(size_t jsize, const std::string& text, size_t line_length,
                  std::ostream& wrapped)
{
    //TODO(dmarchal): All that code is a mess...need to be done for real.

    /// space and * are separator that are returned in the token flow
    /// while "\n" is a 'hidden' separator.
    static char_separator sep("\n", "* '");

    std::string emptyspace(jsize, ' ') ;

    tokenizer tokens(text, sep) ;

    int numspaces = 0 ;
    bool beginOfLine = false ;
    bool isInItalic = false ;

    size_t space_left = line_length;
    for (tokenizer::iterator tok_iter = tokens.begin();tok_iter != tokens.end(); ++tok_iter)
    {
        const std::string& word = *tok_iter;
        if(word=="'" || word=="*")
        {
            if(isInItalic)
            {
                isInItalic=false;
                wrapped << console::Style::Reset;
                wrapped << "'";
                continue;
            }
            else
            {
                isInItalic=true;

                if(numspaces==1){
                    if(!beginOfLine){
                        wrapped << " ";
                        numspaces = 0;
                        space_left--;
                    }else{
                        wrapped << console::Style::Reset << emptyspace ;
                    }
                }

                wrapped << "'";
                wrapped << console::Style::Italic;
                wrapped << console::Style::Underline;
                continue;
            }
        }else if(word==" ")
        {
            if(numspaces==1)
            {
                wrapped << "\n"  ;
                numspaces=0;
                space_left = line_length;
                beginOfLine=true;
                continue;
            }else
            {
                numspaces=1;
                continue;
            }
        }else{
            if(numspaces==1){
                if(!beginOfLine){
                    wrapped << " ";
                    numspaces = 0;
                    space_left--;
                }else{
                    wrapped << console::Style::Reset;
                    wrapped << emptyspace ;
                    if(isInItalic){
                        wrapped << console::Style::Italic;
                        wrapped << console::Style::Underline;
                    }
                }
            }
        }

        if (space_left < word.length() + 1)
        {
            if(word.length()>line_length)
            {
                std::string first;
                size_t curidx=0;
                size_t endidx=std::min(word.length(), space_left-1);

                while(curidx < word.length())
                {
                    first=word.substr(curidx,endidx);

                    if(beginOfLine){
                        wrapped << console::Style::Reset;
                        wrapped << emptyspace ;
                        if(isInItalic){
                            wrapped << console::Style::Italic;
                            wrapped << console::Style::Underline;
                        }
                    }
                    beginOfLine=false;
                    wrapped << first ;

                    curidx+=endidx;
                    endidx=std::min(word.length()-curidx, line_length-1);

                    if(curidx < word.length())
                    {
                        wrapped << "\n" ;
                        beginOfLine=true;
                    }
                }
                space_left = line_length - first.length();
            }
            else
            {
                wrapped << "\n";
                wrapped << console::Style::Reset;
                wrapped << emptyspace ;
                if(isInItalic){
                    wrapped << console::Style::Italic;
                    wrapped << console::Style::Underline;
                }
                wrapped << word ;
                space_left = line_length-word.length();
            }
        }
        else
        {
            if(beginOfLine){
                wrapped << console::Style::Reset;
                wrapped << emptyspace ;
                if(isInItalic){
                    wrapped << console::Style::Italic;
                    wrapped << console::Style::Underline;
                }
            }
            beginOfLine=false;
            wrapped << word;
            space_left -= word.length() ;
        }
    }
}



////////////////////////////// RichConsoleStyleMessageFormatter Implementation /////////////////////

void RichConsoleStyleMessageFormatter::formatMessage(const Message& m, std::ostream& out)
{
    size_t psize = getPrefixText(m.type()).size() ;

    setColor(out, m.type()) << getPrefixText(m.type());

    SofaComponentInfo* nfo = dynamic_cast<SofaComponentInfo*>(m.componentInfo().get()) ;
    if( nfo != nullptr )
    {
        const std::string& classname= nfo->sender();
        const std::string& name = nfo->name();
        psize +=classname.size()+name.size()+5 ;
        out << console::Foreground::Normal::Blue << "[" << classname << "(" << name << ")] ";
    }
    else
    {
        psize +=m.sender().size()+3 ;
        out << console::Foreground::Normal::Blue << "[" << m.sender()<< "] ";
    }

    out << console::Style::Reset;

    /// Format & align the text and write the result into 'out'.
    simpleFormat(psize , m.message().str(), console::getColumnCount()-psize, out) ;

    if(m_showFileInfo && m.fileInfo()){
        std::stringstream buf;
        std::string emptyspace(psize, ' ') ;
        buf << "Emitted from '" << m.fileInfo()->filename << "' line " << m.fileInfo()->line ;
        out << "\n" << console::Style::Reset << emptyspace ;
        simpleFormat(psize , buf.str(), console::getColumnCount()-psize, out) ;
    }

    ///Restore the console rendering attribute.
    out << console::Style::Reset;
    out << std::endl ;
}

} // namespace sofa::helper::logging

namespace { // Anonymous namespace

/** @see boost::empty_token_policy */
enum empty_token_policy { drop_empty_tokens, keep_empty_tokens };

/** see @boost::tokenizer_detail::traits_extension<traits> */
template<typename traits>
struct char_traits : public traits {
    typedef typename traits::char_type char_type;
    static bool isspace(char_type c)
    {
        return static_cast< unsigned >(c) <= 255 && std::isspace(c) != 0;
    }

    static bool ispunct(char_type c)
    {
        return static_cast< unsigned >(c) <= 255 && std::ispunct(c) != 0;
    }
};

/** @see boost::char_separator */
class char_separator
{
    using Char = char;
    using Tr = std::basic_string<Char>::traits_type;
    typedef char_traits<Tr> Traits;
    typedef std::basic_string<Char,Tr> string_type;
public:
    explicit
    char_separator(const Char* dropped_delims,
                   const Char* kept_delims = 0,
                   empty_token_policy empty_tokens = drop_empty_tokens)
            : m_dropped_delims(dropped_delims),
              m_use_ispunct(false),
              m_use_isspace(false),
              m_empty_tokens(empty_tokens),
              m_output_done(false)
    {
        // Borland workaround
        if (kept_delims)
            m_kept_delims = kept_delims;
    }

    // use ispunct() for kept delimiters and isspace for dropped.
    explicit
    char_separator()
            : m_use_ispunct(true),
              m_use_isspace(true),
              m_empty_tokens(drop_empty_tokens),
              m_output_done(false) { }

    void reset() { }

    template <typename InputIterator, typename Token>
    bool operator()(InputIterator& next, InputIterator end, Token& tok)
    {
        typedef tokenizer_detail::assign_or_plus_equal<
                BOOST_DEDUCED_TYPENAME boost::tokenizer_detail::get_iterator_category<
                        InputIterator
                >::iterator_category
        > assigner;

        assigner::clear(tok);

        // skip past all dropped_delims
        if (m_empty_tokens == drop_empty_tokens)
            for (; next != end  && is_dropped(*next); ++next)
            { }

        InputIterator start(next);

        if (m_empty_tokens == drop_empty_tokens) {

            if (next == end)
                return false;


            // if we are on a kept_delims move past it and stop
            if (is_kept(*next)) {
                assigner::plus_equal(tok,*next);
                ++next;
            } else
                // append all the non delim characters
                for (; next != end && !is_dropped(*next) && !is_kept(*next); ++next)
                    assigner::plus_equal(tok,*next);
        }
        else { // m_empty_tokens == keep_empty_tokens

            // Handle empty token at the end
            if (next == end)
            {
                if (m_output_done == false)
                {
                    m_output_done = true;
                    assigner::assign(start,next,tok);
                    return true;
                }
                else
                    return false;
            }

            if (is_kept(*next)) {
                if (m_output_done == false)
                    m_output_done = true;
                else {
                    assigner::plus_equal(tok,*next);
                    ++next;
                    m_output_done = false;
                }
            }
            else if (m_output_done == false && is_dropped(*next)) {
                m_output_done = true;
            }
            else {
                if (is_dropped(*next))
                    start=++next;
                for (; next != end && !is_dropped(*next) && !is_kept(*next); ++next)
                    assigner::plus_equal(tok,*next);
                m_output_done = true;
            }
        }
        assigner::assign(start,next,tok);
        return true;
    }

private:
    string_type m_kept_delims;
    string_type m_dropped_delims;
    bool m_use_ispunct;
    bool m_use_isspace;
    empty_token_policy m_empty_tokens;
    bool m_output_done;

    bool is_kept(Char E) const
    {
        if (m_kept_delims.length())
            return m_kept_delims.find(E) != string_type::npos;
        else if (m_use_ispunct) {
            return Traits::ispunct(E) != 0;
        } else
            return false;
    }
    bool is_dropped(Char E) const
    {
        if (m_dropped_delims.length())
            return m_dropped_delims.find(E) != string_type::npos;
        else if (m_use_isspace) {
            return Traits::isspace(E) != 0;
        } else
            return false;
    }
};

/** @see boost::tokenizer */
class tokenizer {
private:
    using TokenizerFunc = char_separator;
    using Iterator = std::string::const_iterator;
    using  Type = std::string;
    typedef token_iterator_generator<TokenizerFunc,Iterator,Type> TGen;

    // It seems that MSVC does not like the unqualified use of iterator,
    // Thus we use iter internally when it is used unqualified and
    // the users of this class will always qualify iterator.
    typedef typename TGen::type iter;

public:

    typedef iter iterator;
    typedef iter const_iterator;
    typedef Type value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const pointer const_pointer;
    typedef void size_type;
    typedef void difference_type;

    tokenizer(Iterator first, Iterator last,
              const TokenizerFunc& f = TokenizerFunc())
            : first_(first), last_(last), f_(f) { }

    template <typename Container>
    tokenizer(const Container& c)
            : first_(c.begin()), last_(c.end()), f_() { }

    template <typename Container>
    tokenizer(const Container& c,const TokenizerFunc& f)
            : first_(c.begin()), last_(c.end()), f_(f) { }

    void assign(Iterator first, Iterator last){
        first_ = first;
        last_ = last;
    }

    void assign(Iterator first, Iterator last, const TokenizerFunc& f){
        assign(first,last);
        f_ = f;
    }

    template <typename Container>
    void assign(const Container& c){
        assign(c.begin(),c.end());
    }


    template <typename Container>
    void assign(const Container& c, const TokenizerFunc& f){
        assign(c.begin(),c.end(),f);
    }

    iter begin() const { return iter(f_,first_,last_); }
    iter end() const { return iter(f_,last_,last_); }

private:
    Iterator first_;
    Iterator last_;
    TokenizerFunc f_;
};
}
