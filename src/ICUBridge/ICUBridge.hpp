/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#if !defined(ICUBRIDGE_HEADER_GUARD_1357924680)
#define ICUBRIDGE_HEADER_GUARD_1357924680



#include <ICUBridge/ICUBridgeDefinitions.hpp>



#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



// We're stuck here.  We don't want to include the ICU header files, since we're trying
// to keep them hidden, but we need their namespace.  So we're just duplicating here.  When
// it changes, we'll have to track that change.  This is more desirable than forcing users
// to have the ICU sources.
//
// We could fix this by using factories, rather than allowing user access to these
// implementation classes.  It's certainly worth pursuing...
#if defined(XALAN_NO_NAMESPACES)
class UnicodeString;
#else
namespace icu_2_0
{
	class UnicodeString;
};
#endif



class XalanDecimalFormatSymbols;



class XALAN_ICUBRIDGE_EXPORT ICUBridge
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef UnicodeString			ICUUnicodeString;
#else
	typedef icu_2_0::UnicodeString	ICUUnicodeString;
#endif

	static const ICUUnicodeString
	XalanDOMCharStringToUnicodeString(const XalanDOMChar*	theString);

	static const ICUUnicodeString
	XalanDOMStringToUnicodeString(const XalanDOMString&		theString);

	static const XalanDOMString
	UnicodeStringToXalanDOMString(const ICUUnicodeString&	theString);

	static void
	UnicodeStringToXalanDOMString(
			const ICUUnicodeString&		theString,
			XalanDOMString&				theResult);

	static unsigned long
	FormatNumber(
			const XalanDOMString&				thePattern,
			double								theNumber,
			const XalanDecimalFormatSymbols*	theXalanDFS,
			XalanDOMString&						theResult);

	static int
	collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);

	static int
	collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS);
};



#endif	// ICUBRIDGE_HEADER_GUARD_1357924680