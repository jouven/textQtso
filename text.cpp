#include "text.hpp"

textCompilation_c::textCompilation_c(const std::vector<text_c>& texts_par_con)
    : texts_pri(texts_par_con)
{}

void textCompilation_c::append_f(const text_c& text_par_con)
{
    texts_pri.emplace_back(text_par_con);
}

void textCompilation_c::append_f(const textCompilation_c& textCompilationSource_par_con)
{
    texts_pri.insert(texts_pri.end(), textCompilationSource_par_con.texts_pri.begin(), textCompilationSource_par_con.texts_pri.end());
}

text_c textCompilation_c::text_f(const size_t index_par_con) const
{
    text_c resultTmp;
    if (index_par_con < texts_pri.size())
    {
        resultTmp = texts_pri.at(index_par_con);
    }
    return resultTmp;
}

bool textCompilation_c::remove_f(const size_t index_par_con)
{
    bool resultTmp(false);
    if (texts_pri.size() > index_par_con)
    {
        texts_pri.erase(texts_pri.begin() + index_par_con);
        resultTmp = true;
    }
    return resultTmp;
}


QString textCompilation_c::toRawText_f() const
{
    QString resultTmp;
    for (const text_c& text_ite_con : texts_pri)
    {
        resultTmp.append(text_ite_con.rawText_f());
    }
    return resultTmp;
}

QString textCompilation_c::toRawReplace_f(std::vector<std::vector<size_t>>* failReplaceIndexesPtr_par) const
{
    QString resultTmp;
    if (failReplaceIndexesPtr_par not_eq nullptr)
    {
        failReplaceIndexesPtr_par->resize(texts_pri.size());
        for (size_t i = 0, l = texts_pri.size(); i < l; ++i)
        {
            resultTmp.append(texts_pri.at(i).rawReplace_f(nullptr, std::addressof(failReplaceIndexesPtr_par->at(i))));
        }
    }
    else
    {
        for (size_t i = 0, l = texts_pri.size(); i < l; ++i)
        {
            resultTmp.append(texts_pri.at(i).rawReplace_f());
        }
    }
    return resultTmp;
}

void textCompilation_c::clear_f()
{
    texts_pri.clear();
}

size_t textCompilation_c::size_f() const
{
    return texts_pri.size();
}

size_t textCompilation_c::rawTextSize_f() const
{
    size_t resultTmp(0);
    for (const text_c& text_ite_con : texts_pri)
    {
        resultTmp = resultTmp + text_ite_con.rawTextSize_f();
    }
    return resultTmp;
}

bool textCompilation_c::empty_f() const
{
    return texts_pri.empty();
}

bool textCompilation_c::operator ==(const QString& string_par_con) const
{
    return toRawText_f() == string_par_con;
}

bool textCompilation_c::operator !=(const QString& string_par_con) const
{
    return toRawText_f() not_eq string_par_con;
}

void textCompilation_c::insertIndex_f(
        const text_c& text_par_con
        , const uint_fast64_t index_par_con)
{
    while (true)
    {
        if (index_par_con >= texts_pri.size())
        {
            append_f(text_par_con);
            break;
        }

        if (index_par_con < texts_pri.size())
        {
            //get current last index
            const uint_fast64_t lastIndexTmp_con(texts_pri.size() - 1);
            //resize vector
            texts_pri.resize(texts_pri.size() + 1);
            //copy each index to index + 1
            for (uint_fast64_t indexFromTheEnd_ite = lastIndexTmp_con; indexFromTheEnd_ite >= index_par_con; --indexFromTheEnd_ite)
            {
                texts_pri.at(indexFromTheEnd_ite + 1) = texts_pri.at(indexFromTheEnd_ite);
            }
            texts_pri.at(index_par_con) = text_par_con;
            break;
        }
    }
}

bool textCompilation_c::operator !=(const textCompilation_c& textCompilation_par_con) const
{
    return texts_pri not_eq textCompilation_par_con.texts_pri;
}

bool textCompilation_c::operator ==(const textCompilation_c& textCompilation_par_con) const
{
    return texts_pri == textCompilation_par_con.texts_pri;
}



bool isValidStringSize_f(
        const QString& str_par_con
        , const int_fast32_t maxSize_par_con
        //where the error message will be assigned (if not nullptr)
        , text_c* errorTextPtr_par
        //{0} will be replaced by str_par_con.size()
        //{1} will be replaced by maxSize_par_con
        //still this the default, it can be replaced with another customized string,
        //replace can be done outside too
        , const QString& errorMessage_par_con)
{
    bool isValidTmp(false);
    while (true)
    {
        if (str_par_con.size() > maxSize_par_con)
        {
            if (errorTextPtr_par not_eq nullptr)
            {
                *errorTextPtr_par = text_c(errorMessage_par_con, str_par_con.size(), maxSize_par_con);
            }
            break;
        }

        isValidTmp = true;
        break;
    }
    return isValidTmp;
}

