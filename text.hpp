#ifndef TRANSLATATORJSONQTSO_TEXT_HPP
#define TRANSLATATORJSONQTSO_TEXT_HPP

#include <QString>
#ifdef DEBUGJOUVEN
#include <QDebug>
#endif

#include <deque>
#include <vector>

//the idea behind these classes
//is to be used in libraries that don't know initially
//if a translation is required, but need to separate
//the translatable text from the "values" which won't be translated
//but at some point need to be merged-replace the text
//to make a meaningful message
class text_c
{
    QString text_pri;

    //because text_c is used in the translation library
    //without this when a text_c is translated it can't be known
    //if it's stored and then loaded some time later
    //this solves the issue also works as an "optimization" because
    //translations of "translated" text_c can be skipped
    bool translated_pri = false;

    //replace index start at 0 = "{0}"
    //replacemens are applied in descending index order (last index is replaced, then last - 1... until {0})
    //a deque is used because recursion inserts the replacements in "reverse" (the last is the first inserted)
    std::deque<QString> replacements_pri;

    void addReplacement_f(const QString& string_par_con)
    {
#ifdef DEBUGJOUVEN
        //qDebug() << "string_par_con " << string_par_con << endl;
#endif
        replacements_pri.emplace_front(string_par_con);
    }
    void addReplacement_f(const int_fast64_t int_par_con)
    {
#ifdef DEBUGJOUVEN
        //qDebug() << "int_par_con " << int_par_con << endl;
#endif
        replacements_pri.emplace_front(QString::number(int_par_con));
    }

    QString frontSeparator_pri = "{";
    QString backSeparator_pri = "}";

    template<typename T>
    void addReplacementRec_f(T value_par)
    {
#ifdef DEBUGJOUVEN
        //qDebug() << "last level addReplacementVar_f value_par " << value_par << endl;
#endif
        addReplacement_f(value_par);
    }

    template<typename T, typename... Rargs>
    void addReplacementRec_f(T value_par, Rargs... rargs)
    {
#ifdef DEBUGJOUVEN
        //qDebug() << "template addReplacement_f value_par " << value_par << endl;
        //qDebug() << "template addReplacement_f sizeof... (Fargs) " << sizeof... (Fargs) << endl;
#endif
        addReplacementRec_f(rargs...);
        addReplacement_f(value_par);
    }
public:
    //for qregistermetatype (when using this class in signal-slot)
    explicit text_c() = default;

    //only string/text no replacements case
    template<typename T>
    text_c(T value_par)
    {
#ifdef DEBUGJOUVEN
        //qDebug() << "base ctor value_par " << value_par << endl;
#endif
        text_pri = value_par;
        //addReplacement_f(value_par);
    }

    //First argument is always the text and is mandatory, everything after are replacement values
    template<typename T, typename... Rargs>
    text_c(T value_par, Rargs... rargs)
        : text_pri(value_par)
    {
#ifdef DEBUGJOUVEN
        //qDebug() << "template ctor value_par " << value_par << endl;
        //qDebug() << "template ctor sizeof... (Fargs) " << sizeof... (Fargs) << endl;
#endif
        addReplacementRec_f(rargs...);

        //WRONG, creates a temporary class
        //text_c(Fargs...)
    }
    QString frontSeparator_f() const
    {
        return frontSeparator_pri;
    }
    QString backSeparator_f() const
    {
        return backSeparator_pri;
    }
    void setSeparators_f(
            const QString& frontSeparator_par_con
            , const QString& backSeparator_par_con)
    {
        if (frontSeparator_par_con.isEmpty() or backSeparator_pri.isEmpty())
        {
            //don't change them if any is set to empty
        }
        else
        {
            frontSeparator_pri = frontSeparator_par_con;
            backSeparator_pri = backSeparator_par_con;
        }
    }

    size_t rawTextSize_f() const
    {
        return text_pri.size();
    }
    QString rawText_f() const
    {
        return text_pri;
    }

    //string can be an external string to be replaced, if null text_pri will be used
    QString rawReplace_f(const QString* const string_par_con = nullptr, std::vector<size_t>* failReplaceIndexesPtr_par = nullptr) const
    {
        QString resultTmp(string_par_con == nullptr ? text_pri : *string_par_con);
        if (replacements_pri.empty())
        {
            //nothing to do here
        }
        else
        {
            //replace from the last index to the first index
            for (int_fast64_t index_ite = (replacements_pri.size() - 1); index_ite > -1; --index_ite)
            {
                if (failReplaceIndexesPtr_par not_eq nullptr)
                {
                    QString currentValue(resultTmp);
                    resultTmp.replace(frontSeparator_pri + QString::number(index_ite) + backSeparator_pri, replacements_pri.at(index_ite));
                    //if the string before the replace is the same after the replace
                    //or the replace value is "{i}" (or the equivalent-changed placeholder)
                    //or the replace didn't happen (the two "or the replace..." imply he fist statement anyway)
                    if (currentValue == resultTmp)
                    {
                        failReplaceIndexesPtr_par->emplace_back(index_ite);
                    }
                }
                else
                {
                    resultTmp.replace(frontSeparator_pri + QString::number(index_ite) + backSeparator_pri, replacements_pri.at(index_ite));
                }
            }
        }
        return resultTmp;
    }
    bool operator<(const text_c& textBlock_par_con) const
    {
        return rawReplace_f() < textBlock_par_con.rawReplace_f();
    }
    bool operator<(const QString& string_par_con) const
    {
        return rawReplace_f() < string_par_con;
    }
    bool operator ==(const text_c& textBlock_par_con) const
    {
        return rawReplace_f() == textBlock_par_con.rawReplace_f();
    }
    bool operator ==(const QString& string_par_con) const
    {
        return rawReplace_f() == string_par_con;
    }
    bool operator not_eq(const text_c& textBlock_par_con) const
    {
        return rawReplace_f() not_eq textBlock_par_con.rawReplace_f();
    }
    bool operator not_eq(const QString& string_par_con) const
    {
        return not text_c::operator==(string_par_con);
    }
    bool translated_f() const
    {
        return translated_pri;
    }
    void setTranslated_f(const bool translated_par_con)
    {
        translated_pri = translated_par_con;
    }
    bool empty_f() const
    {
        return text_pri.isEmpty();
    }
};


//several texts (i.e. error texts from different errors)
class textCompilation_c
{
    std::vector<text_c> texts_pri;
public:
    textCompilation_c() = default;
    textCompilation_c(const std::vector<text_c>& texts_par_con);

    void append_f(const text_c& text_par_con);
    void append_f(const textCompilation_c& textCompilationSource_par_con);
    text_c text_f(const size_t index_par_con) const;
    bool remove_f(const size_t index_par_con);

    //returns all the texts concatenated
    QString toRawText_f() const;
    //returns all the texts replaced and concatenated
    //outer vector index is for std::vector<text_c> (a text index)
    //inner for std::deque<QString> replacements_pri; (the replacement index, of that text)
    //failReplaceIndexesPtr_par nullptr it will be ignored, failReplaceIndexesPtr_par resizing will be done accordingly
    QString toRawReplace_f(std::vector<std::vector<size_t>>* failReplaceIndexesPtr_par = nullptr) const;

    void clear_f();
    size_t size_f() const;
    size_t rawTextSize_f() const;
    bool empty_f() const;

    bool operator ==(const textCompilation_c& textCompilation_par_con) const;
    bool operator ==(const QString& string_par_con) const;
    bool operator not_eq(const textCompilation_c& textCompilation_par_con) const;
    bool operator not_eq(const QString& string_par_con) const;

    //insert at index
    //if larger than any existing index it will insert at the last position
    //else it will copy all the text_c from that index to index+1 and insert the text_c to the index
    //can't be negative =)
    //sub-optimal, it "moves" all the existing values in the vector from that index (included) to the end then replaces the index
    void insertIndex_f(const text_c& text_par_con, const uint_fast64_t index_par_con);
};

bool isValidStringSize_f(
        const QString& str_par_con
        , const int_fast32_t maxSize_par_con
        //where the error message will be assigned (if not nullptr)
        , text_c* errorTextPtr_par = nullptr
        //{0} will be replaced by str_par_con.size()
        //{1} will be replaced by maxSize_par_con
        //still this the default, it can be replaced with another customized string,
        //replace can be done outside too
        , const QString& errorMessage_par_con = "String is too long: {0} (maximum length is {1})");


#endif // TRANSLATATORJSONQTSO_TEXT_HPP
