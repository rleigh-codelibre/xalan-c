/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XPATHPROCESSORIMPL_HEADER_GUARD_1357924680)
#define XPATHPROCESSORIMPL_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <set>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <XPath/XPathProcessor.hpp>



#include <XPath/XPath.hpp>



class XalanNode;



/**
 * The XPathProcessorImpl class responsibilities include tokenizing and
 * parsing the XPath expression, and acting as a general interface to XPaths.
 */
class XALAN_XPATH_EXPORT XPathProcessorImpl : public XPathProcessor
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString,
				int,
				less<XalanDOMString> >		KeywordsMapType;
	typedef map<XalanDOMString,
				XPathExpression::eOpCodes,
				less<XalanDOMString> >		FunctionNameMapType;
	typedef map<XalanDOMString,
				XPathExpression::eOpCodes,
				less<XalanDOMString> >		AxisNamesMapType;
	typedef map<XalanDOMString,
				XPathExpression::eOpCodes,
				less<XalanDOMString> >		NodeTypesMapType;

	typedef vector<XalanDOMString>			DOMStringVectorType;

	typedef vector<bool>					BoolVectorType;
#else
	typedef std::map<XalanDOMString,
					 int>							KeywordsMapType;
	typedef std::map<XalanDOMString,
					 XPathExpression::eOpCodes>		FunctionNameMapType;
	typedef std::map<XalanDOMString,
					 XPathExpression::eOpCodes>		AxisNamesMapType;
	typedef std::map<XalanDOMString,
					 XPathExpression::eOpCodes>		NodeTypesMapType;

	typedef std::vector<XalanDOMString>				DOMStringVectorType;

	typedef std::vector<bool>						BoolVectorType;
#endif

	/**
	 * Perform static initialization.  See class XPathInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XPathInit.
	 */
	static void
	terminate();

	explicit
	XPathProcessorImpl();

	virtual
	~XPathProcessorImpl();


	// These are inherited from XPathProcessor...

	virtual void
	initXPath(
			XPath&					pathObj,
			const XalanDOMString&	expression,
			const PrefixResolver&	prefixResolver,
			const Locator*			locator = 0);

	virtual void
	initMatchPattern(
			XPath&					pathObj,
			const XalanDOMString&	expression,
			const PrefixResolver&	prefixResolver,
			const Locator*			locator = 0);

private:

	/**
	 * Walk through the expression and build a token queue, and a map of the
	 * top-level elements.
	 *
	 * @param pat XSLT Expression.
	 * @param targetStrings Vector to hold Strings, may be null.
	 */
	void
	tokenize(
			const XalanDOMString&	pat,
			DOMStringVectorType*	targetStrings = 0);
  
	/**
	 * Record the current position on the token queue as long as this is a
	 * top-level element.  Must be called before the next token is added to the
	 * m_tokenQueue.
	 */
	bool
	mapPatternElemPos(
			int		nesting,
			bool	isStart,
			bool	isAttrName) const;

	/**
	 * Record the correct token string in the passed vector.
	 */
	void
	recordTokenString(DOMStringVectorType&	targetStrings);

	void
	addToTokenQueue(const XalanDOMString&	s) const;

	/**
	 * When a separator token is found, see if there's a element name or the
	 * like to map.
	 */
	int
	mapNSTokens(
			const XalanDOMString&	pat,
			int						startSubstring,
			int						posOfNSSep,
			int						posOfScan) const;

	/**
	 * Given a map pos, return the corresponding token queue pos.
	 */
	int
	getTokenQueuePosFromMap(int		i) const;

	/**
	 * This will return the index above the passed index that is the target
	 * element, i.e. it holds a value >= TARGETEXTRA. If there is no next
	 * target, it will return -1. Pass -1 in to start testing from zero.
	 */
	int
	getNextTargetIndexInMap(int		i) const;
  
	/**
	 * This will return the normalized index into the pattern 
	 * map above the passed index, or -1 if it is the last pattern.
	 */
	int
	getNextIndexInMap(int	i) const;
  
	/**
	 * This will return the index above the passed index that 
	 * is the start if the next subpattern, or -1 if there is none.
	 * If there is no next target, it will return -1.
	 * Pass -1 in to start testing from zero.
	 */
	int
	getNextSubpatternStartIndexInMap(int	i) const;

	/**
	 * This will return the next index from the passed index,
	 * or -1 if it the passed index is the last index of the 
	 * subpattern.
	 */
	int
	getNextPatternPos(int	i) const;

	/**
	 * This will return the previous index from the passed index,
	 * or -1 if it the passed index is the first index of the 
	 * subpattern.
	 */
	int
	getPrevMapIndex(int		i) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(const XalanDOMString&	s) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(const XalanDOMChar*		s) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(const char*		s) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(char	c) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			XalanDOMChar	c,
			int				n) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			const XalanDOMChar*		s,
			int						n) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			const XalanDOMString&	s,
			int						n) const;

	/**
	 * Lookbehind the first character of the current token in order to 
	 * make a branching decision.
	 * @param c the character to compare it to.
	 * @param n number of tokens to lookbehind.  Must be 
	 * greater than 1.  Note that the lookbehind terminates 
	 * at either the beginning of the string or on a '|' 
	 * character.  Because of this, this method should only
	 * be used for pattern matching.
	 */
	bool
	lookbehind( 
			char	c,
			int		n) const;
 
	/**
	 * look behind the current token in order to 
	 * see if there is a useable token.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.  Note that the lookbehind terminates 
	 * at either the beginning of the string or on a '|' 
	 * character.  Because of this, this method should only
	 * be used for pattern matching.
	 * @return true if lookbehind has a token, false otherwise.
	 */
	bool
	lookbehindHasToken(int	n) const;

	/**
	 * Retrieve the next token from the command and
	 * store it in m_token string.
	 */
	void
	nextToken();

	/**
	 * Retrieve the next token from the command and
	 * store it in m_token string.
	 */
	const XalanDOMString&
	getTokenRelative(int	theOffset) const;

	/**
	 * Retrieve the previous token from the command and
	 * store it in m_token string.
	 */
	void
	prevToken();

	/**
	 * Reset token queue mark and m_token to a 
	 * given position.
	 */
	void
	resetTokenMark(int	mark);

	/**
	 * Consume an expected token, throwing an exception if it 
	 * isn't there.
	 */
	void
	consumeExpected(const char*		expected);

	/**
	 * Consume an expected token, throwing an exception if it 
	 * isn't there.
	 */
	void
	consumeExpected(char	expected);

	bool
	isCurrentLiteral() const;

	/**
	 * Determine if the token is an axis
	 *
	 * @param theToken The token to test
	 * @return true if the token is a valid axis, false if not.
	 */
	static bool
	isAxis(const XalanDOMString&	theToken);

	/**
	 * Determine if the token could be a node test
	 *
	 * @param theToken The token to test
	 * @return true if the token is a valid node test, false if not.
	 */
	static bool
	isNodeTest(const XalanDOMString&	theToken);

	/**
	 * Throw an exception using the provided message text.
	 */
	void
	error(
			const XalanDOMString&	msg,
			XalanNode*				sourceNode = 0) const;

	/**
	 * Throw an exception using the provided message text.
	 */
	void
	error(
			const char*		msg,
			XalanNode*		sourceNode = 0) const;

	/**
	 * Given a string, return the corresponding token.
	 */
	int
	getKeywordToken(const XalanDOMString&	key) const;

	/**
	 * Given a string, return the corresponding token.
	 */
	int
	getFunctionToken(const XalanDOMString&	key) const;

	/**
	 * 
	 * --------------------------------------------------------------------------------
	Expr  ::=  OrExpr   
	 * --------------------------------------------------------------------------------
	 */
	void
	Expr();


	/**
	 * 
	 * --------------------------------------------------------------------------------
	 OrExpr  ::=	AndExpr   
	 | OrExpr 'or' AndExpr   
	 * --------------------------------------------------------------------------------
	 */
	void
	OrExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 AndExpr	::=  EqualityExpr	
	 | AndExpr 'and' EqualityExpr   
	 * --------------------------------------------------------------------------------
	 */
	void
	AndExpr() ;
  
	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 EqualityExpr  ::=  RelationalExpr   
	 | EqualityExpr '=' RelationalExpr   
	 * --------------------------------------------------------------------------------
	 */
	int
	EqualityExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 RelationalExpr  ::=	AdditiveExpr   
	 | RelationalExpr '<' AdditiveExpr   
	 | RelationalExpr '>' AdditiveExpr   
	 | RelationalExpr '<=' AdditiveExpr	
	 | RelationalExpr '>=' AdditiveExpr	
	 * --------------------------------------------------------------------------------
	 */
	int
	RelationalExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 AdditiveExpr  ::=  MultiplicativeExpr   
	 | AdditiveExpr '+' MultiplicativeExpr   
	 | AdditiveExpr '-' MultiplicativeExpr   
	 * --------------------------------------------------------------------------------
	 */
	int
	AdditiveExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 MultiplicativeExpr  ::=	UnaryExpr	
	 | MultiplicativeExpr MultiplyOperator UnaryExpr	 
	 | MultiplicativeExpr 'div' UnaryExpr   
	 | MultiplicativeExpr 'mod' UnaryExpr   
	 | MultiplicativeExpr 'quo' UnaryExpr   
	 * --------------------------------------------------------------------------------
	 */
	int
	MultiplicativeExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 UnaryExpr  ::=  UnionExpr   
	 | '-' UnaryExpr	 
	 * --------------------------------------------------------------------------------
	 */
	void
	UnaryExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 BooleanExpr  ::=	Expr   
	 * --------------------------------------------------------------------------------
	 */
	void
	BooleanExpr();

	/**
	 * The context of the right hand side expressions is the context of the 
	 * left hand side expression. The results of the right hand side expressions 
	 * are node sets. The result of the left hand side UnionExpr is the union 
	 * of the results of the right hand side expressions.
	 * 
	 * --------------------------------------------------------------------------------
	 UnionExpr	::=    PathExpr   
	 | UnionExpr '|' PathExpr 	
	 * --------------------------------------------------------------------------------
	 */
	void
	UnionExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 PathExpr  ::=  LocationPath	 
	 | FilterExpr   
	 | FilterExpr '/' RelativeLocationPath   
	 | FilterExpr '//' RelativeLocationPath	
	 * --------------------------------------------------------------------------------
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void
	PathExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 FilterExpr  ::=	PrimaryExpr   
	 | FilterExpr Predicate	
	 * --------------------------------------------------------------------------------
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void
	FilterExpr();
  
	/**
	 * --------------------------------------------------------------------------------
	 PrimaryExpr	::=  VariableReference	 
	 | '(' Expr ')'	
	 | Literal   
	 | Number   
	 | FunctionCall	
	 * --------------------------------------------------------------------------------
	 */
	void
	PrimaryExpr();


	/**
	 * --------------------------------------------------------------------------------
	 Argument    ::=	  Expr	   
	 * --------------------------------------------------------------------------------
	 */
	void
	Argument();
  
	/**
	 * --------------------------------------------------------------------------------
	 FunctionCall    ::=	  FunctionName '(' ( Argument ( ',' Argument)*)? ')'	
	 * --------------------------------------------------------------------------------
	 */
	void
	FunctionCall();

	/**
	 * --------------------------------------------------------------------------------
	 LocationPath ::= RelativeLocationPath 
	 | AbsoluteLocationPath 
	 * --------------------------------------------------------------------------------
	 */
	void
	LocationPath();
  
	/**
	 * --------------------------------------------------------------------------------
	 RelativeLocationPath ::= Step 
	 | RelativeLocationPath '/' Step 
	 | AbbreviatedRelativeLocationPath 
	 * --------------------------------------------------------------------------------
	 */
	void
	RelativeLocationPath();
  
	/**
	 * --------------------------------------------------------------------------------
	 Step    ::=	  Basis Predicate*	
	 | AbbreviatedStep
	 */
	void
	Step();
  
	/**
	 * --------------------------------------------------------------------------------
	 Basis	::=    AxisName '::' NodeTest	
	 | AbbreviatedBasis  
	 */
	void
	Basis();
  
	/**
	 * --------------------------------------------------------------------------------
	 Basis	::=    AxisName '::' NodeTest	
	 | AbbreviatedBasis  
	 */
	int
	AxisName();
  
	/**
	 * --------------------------------------------------------------------------------
	 NodeTest    ::=	  WildcardName	 
	 | NodeType '(' ')'  
	 | 'processing-instruction' '(' Literal ')' 
	 */
	void
	NodeTest(int	axisType);

	/**
	 * --------------------------------------------------------------------------------
	 Predicate ::= '[' PredicateExpr ']' 
	 * --------------------------------------------------------------------------------
	 */
	void
	Predicate();

	/**
	 *--------------------------------------------------------------------------------
	 PredicateExpr ::= Expr 
	 *--------------------------------------------------------------------------------
	 */
	void
	PredicateExpr();
  
	/**
	 * QName ::=	(Prefix ':')? LocalPart 
	 * Prefix ::=  NCName 
	 * LocalPart ::=	NCName 
	 */
	void
	QName();

	/**
	 * NCName ::=  (Letter | '_') (NCNameChar)*
	 * NCNameChar ::=  Letter | Digit | '.' | '-' | '_' | CombiningChar | Extender 
	 */
	void
	NCName();
  
	/**
	 * The value of the Literal is the sequence of characters inside 
	 * the " or ' characters>.
	 * --------------------------------------------------------------------------------
	 Literal	::=  '"' [^"]* '"'   
	 | "'" [^']* "'"	 
	 * --------------------------------------------------------------------------------
	 */
	void
	Literal();
  
	/**
	 * --------------------------------------------------------------------------------
	 * Number ::= [0-9]+('.'[0-9]+)? | '.'[0-9]+
	 * --------------------------------------------------------------------------------
	 */
	void
	Number();

	/**
	 * --------------------------------------------------------------------------------
	 Pattern	::=  LocationPathPattern   
	 | Pattern '|' LocationPathPattern   
	 * --------------------------------------------------------------------------------
	 */
	void
	Pattern();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 LocationPathPattern	::=  '/' RelativePathPattern?	
	 | IdKeyPattern (('/' | '//') RelativePathPattern)?	
	 | '//'? RelativePathPattern	 
	 * --------------------------------------------------------------------------------
	 */
	void
	LocationPathPattern();

	/**
	 * --------------------------------------------------------------------------------
	 IdKeyPattern  ::=  'id' '(' Literal ')'	 
	 | 'key' '(' Literal ',' Literal ')'	 
	 * (Also handle doc())
	 * --------------------------------------------------------------------------------
	 */
	void
	IdKeyPattern();

	/**
	 * --------------------------------------------------------------------------------
	 RelativePathPattern	::=  StepPattern   
	 | RelativePathPattern '/' StepPattern   
	 | RelativePathPattern '//' StepPattern	
	 * --------------------------------------------------------------------------------
	 */
	void
	RelativePathPattern();

	/**
	 * --------------------------------------------------------------------------------
	 StepPattern	::=  AbbreviatedNodeTestStep  
	 * --------------------------------------------------------------------------------
	 */
	void
	StepPattern();

	/**
	 * --------------------------------------------------------------------------------
	 AbbreviatedNodeTestStep	  ::=	 '@'? NodeTest Predicate*	
	 * --------------------------------------------------------------------------------
	 */
	void
	AbbreviatedNodeTestStep();

	bool
	isValidFunction(const XalanDOMString&	key) const;

private:

	int
	FunctionCallArguments();

	static void
	initializeKeywordsTable(KeywordsMapType&	theKeywords);

	static void
	initializeFunctionTable(FunctionNameMapType&	theFunctions);

	static void
	initializeAxisNamesTable(AxisNamesMapType&		theAxisNames);

	static void
	initializeNodeTypesTable(NodeTypesMapType&		theNodeTypes);

	/**
	 * The current input token.
	 */
	XalanDOMString					m_token;
  
	/**
	 * The first char in m_token, the theory being that this 
	 * is an optimization because we won't have to do charAt(0) as
	 * often.
	 */
	XalanDOMChar					m_tokenChar;

	/**
	 * A pointer to the current XPath.
	 */
	XPath*							m_xpath;

	/**
	 * A pointer to the current XPath's expression.
	 */
	XPathExpression*				m_expression;

	/**
	 * A pointer to the current executionContext.
	 */
	const PrefixResolver*			m_prefixResolver;

	bool							m_requireLiterals;

	bool							m_isMatchPattern;

	const Locator*					m_locator;

	BoolVectorType					m_positionPredicateStack;

	enum eDummy
	{
		TARGETEXTRA = 10000
	};

	static const XalanDOMString		s_emptyString;

	// This shouldn't really be here, since it duplicates a string that is part
	// of the information that is maintained by the class XPathFunctionTable,
	// but this is a reasonable optimization.
	static const XalanDOMString&	s_functionIDString;


	// This shouldn't really be here, since it's not part of the XPath standard,
	// but rather a part ofthe XSLT standard.
	static const XalanDOMString&	s_functionKeyString;

	static const XalanDOMString&	s_orString;

	static const XalanDOMString&	s_andString;

	static const XalanDOMString&	s_divString;

	static const XalanDOMString&	s_modString;

	static const XalanDOMString&	s_dotString;

	static const XalanDOMString&	s_dotDotString;

	static const XalanDOMString&	s_axisString;

	static const XalanDOMString&	s_attributeString;

	static const XalanDOMString&	s_childString;

	static const XalanDOMString&	s_positionString;

	/**
	 * Map of keyword names to token values.
	 */
	static const KeywordsMapType&		s_keywords;

	static const FunctionNameMapType&	s_functions;

	static const AxisNamesMapType&		s_axisNames;

	static const NodeTypesMapType&		s_nodeTypes;
};



#endif	// XPATHPROCESSORIMPL_HEADER_GUARD_1357924680