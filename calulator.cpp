#pragma once
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>


using namespace boost::spirit::qi;
using namespace std;
using namespace boost::spirit;


template <typename Iterator, typename NumType>
struct CalcGrammar : public grammar<Iterator, NumType(), space_type>
{
	CalcGrammar() : CalcGrammar::base_type(exp_)
	{
		real_parser<NumType, real_policies<NumType> > rp_;
		factor_ = rp_[_val = _1]
			| ('-' >> factor_)[_val = -_1]
			| ('-' >> factor_)[_val = _1]
			| ('(' >> exp_[_val = _1] >> ')');

		term_ = factor_[_val = _1] >>
			*(('*' >> factor_[_val *= _1])
			| ('/' >> factor_[_val /= _1]));

		exp_ = term_[_val = _1] >>
			*(('+' >> term_[_val += _1])
			| ('-' >> term_[_val -= _1]));
	}
	typedef rule<Iterator, NumType(), space_type> rule_t;
	rule_t factor_, term_, exp_;
};


template<class _NumType = double>
class Calculator
{
public:
	Calculator(void){}
	virtual ~Calculator(void){}
public:
	typedef _NumType NumType;
public:
	_NumType calc(const std::string& input);
	_NumType calc(const std::string& input,bool& isSuccessed,const char*& lastErr);
	const char* getLastErr(){ return lastErr_.c_str(); }
protected:
	CalcGrammar<std::string::const_iterator, _NumType> gram_;
	std::string lastErr_;
};
template<class _NumType>
_NumType Calculator<_NumType>::calc(const std::string& input)
{
	bool isSuccess;
	const char* lastErr = nullptr;
	return calc(input, isSuccess, lastErr);
}

template<class _NumType>
_NumType Calculator<_NumType>::calc(const std::string& input, bool& isSuccessed, const char*& lastErr)
{
	_NumType val = 0;
	std::string::const_iterator startPos = input.begin();
	auto r = phrase_parse(startPos, input.end(), gram_, space, val);
	if (r && startPos == input.end())
	{
		lastErr_.clear();
		isSuccessed = true;
		lastErr = "";
		return val;
	}
	else
	{
		int num = input.end() - startPos;
		lastErr_ = std::string(num, '-') + "^";
		isSuccessed = false;
		lastErr = lastErr_.c_str();
	}
}

int main()
{
	Calculator<> calc;
	std::string input;
	printf("input here\n");
	while (true)
	{
		getline(cin,input);
		if (input == "")
			break;
		int val = calc.calc(input.c_str());
		if (calc.getLastErr()[0] != 0)
			printf("%s\nWrong\n\n", calc.getLastErr());
		else
			printf("%d\n\n", val);
	}
}
