/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include "ElemElement.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemElement::ElemElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			StylesheetConstructionContext::ELEMNAME_ELEMENT),
	m_nameAVT(0),
	m_namespaceAVT(0)
{
	// Namespace aliases are not used for xsl:element, so
	// turn them off...
//	m_namespacesHandler.setProcessNamespaceAliaises(false);

	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_nameAVT =
				constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_NAMESPACE))
		{
			m_namespaceAVT =
				constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this); 
		}
		else if(!(processUseAttributeSets(constructionContext, aname, atts, i) ||
			    processSpaceAttr(aname, atts, i, constructionContext) ||
				isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_nameAVT)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::TemplateMustHaveAttribute_2Param,"xsl:element","name"),
			0,
			this);
	}
}



ElemElement::~ElemElement()
{
}



const XalanDOMString&
ElemElement::getElementName() const
{
	return Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING;
}



void
ElemElement::execute(StylesheetExecutionContext&		executionContext) const
{
	StylesheetExecutionContext::GetAndReleaseCachedString	elemNameGuard(executionContext);

	XalanDOMString&		elemName = elemNameGuard.get();

	m_nameAVT->evaluate(elemName, *this, executionContext);

	bool	isIllegalElement = !XalanQName::isValidQName(elemName);

	if (isIllegalElement == true)
	{
		executionContext.warn(
			XalanMessageLoader::getMessage(XalanMessages::IllegalElementName_1Param, elemName),
			executionContext.getCurrentNode(),
			getLocator());

		ElemUse::doExecute(executionContext, false);

		doExecuteChildren(executionContext, true);
	}
	else
	{
		StylesheetExecutionContext::GetAndReleaseCachedString	elemNameSpaceGuard(executionContext);

		XalanDOMString&		elemNameSpace = elemNameSpaceGuard.get();

		if (m_namespaceAVT != 0)
		{
			m_namespaceAVT->evaluate(elemNameSpace, *this, executionContext);
		}

		XalanDOMString::size_type	namespaceLen = length(elemNameSpace);

		bool	foundResultNamespaceForPrefix = false;

		XalanDOMString::size_type			len = length(elemName);

		const XalanDOMString::size_type		indexOfNSSep = indexOf(elemName, XalanUnicode::charColon);

		const bool	havePrefix = indexOfNSSep == len ? false : true;

		StylesheetExecutionContext::GetAndReleaseCachedString	prefixGuard(executionContext);

		XalanDOMString&		prefix = prefixGuard.get();

		if (havePrefix == true)
		{
			substring(elemName, prefix, 0, indexOfNSSep);

			const XalanDOMString* const		theNamespace =
				executionContext.getResultNamespaceForPrefix(prefix);

			if (theNamespace != 0)
			{
				foundResultNamespaceForPrefix = true;
			}
			else
			{
				const XalanDOMString* const		theNamespace =
					getNamespacesHandler().getNamespace(prefix);

				if(theNamespace == 0 && namespaceLen == 0)
				{
					executionContext.warn(
						XalanMessageLoader::getMessage(
							XalanMessages::CannotResolvePrefix_1Param,
							prefix),
						executionContext.getCurrentNode(),
						getLocator());

					if (m_namespaceAVT != 0)
					{
						elemName.erase(0, indexOfNSSep + 1);
					}
					else
					{
						isIllegalElement = true;

						executionContext.warn(
							XalanMessageLoader::getMessage(
								XalanMessages::IllegalElementName_1Param,
								elemName),
							executionContext.getCurrentNode(),
							getLocator());
					}
				}
				else if (theNamespace != 0 &&
						 namespaceLen == 0 &&
						 equals(prefix, DOMServices::s_XMLNamespace) == false)
				{
					elemNameSpace = *theNamespace;
				}
			}
		}

		if (isIllegalElement == false)
		{
			executionContext.startElement(c_wstr(elemName));   

			if(0 == m_namespaceAVT &&
			   (havePrefix == false || foundResultNamespaceForPrefix == true))
			{
				if (havePrefix == false)
				{
					fixupDefaultNamespace(executionContext);
				}
			}
			else
			{
				if(havePrefix == false)
				{
					if (namespaceLen > 0)
					{
						const XalanDOMString* const		theDefaultNamespace =
								executionContext.getResultNamespaceForPrefix(s_emptyString);

						if (theDefaultNamespace == 0 ||
							equals(*theDefaultNamespace, elemNameSpace) == false)
						{
							executionContext.addResultAttribute(
									DOMServices::s_XMLNamespace,
									elemNameSpace);
						}
					}
					else
					{
						// OK, the namespace we're generating is the default namespace,
						// so let's make sure that we really need it.  If we don't,
						// we end up with another xmlns="" on the element we're
						// generating.  Although this isn't really an error, it's
						// a bit unsightly, so let's suppress it...
						const XalanDOMString&	theParentDefaultNamespace =
								getParentDefaultNamespace();

						if (length(theParentDefaultNamespace) == 0)
						{
							if (executionContext.getResultNamespaceForPrefix(s_emptyString) != 0)
							{
								executionContext.addResultAttribute(DOMServices::s_XMLNamespace, elemNameSpace);
							}
						}
						else
						{
							executionContext.addResultAttribute(DOMServices::s_XMLNamespace, elemNameSpace);
						}
					}
				}
				else
				{
					const XalanDOMString* const		theNamespace =
							executionContext.getResultNamespaceForPrefix(prefix);

					if (theNamespace == 0 ||
						equals(*theNamespace, elemNameSpace) == false)
					{
						insert(prefix, 0, DOMServices::s_XMLNamespaceWithSeparator);

						executionContext.addResultAttribute(prefix, elemNameSpace);
					}
				}
			}
		}

		if (isIllegalElement == true)
		{
			ElemUse::doExecute(executionContext, false);

			doExecuteChildren(executionContext, true);
		}
		else
		{
			ElemUse::doExecute(executionContext, true);

			doExecuteChildren(executionContext, false);

			executionContext.endElement(c_wstr(elemName));
		}
	}
}



void
ElemElement::namespacesPostConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler,
			NamespacesHandler&				theHandler)
{
	theHandler.postConstruction(
			constructionContext,
			false,
			getElementName(),
			&theParentHandler);
}



void
ElemElement::doExecuteChildren(
			StylesheetExecutionContext&		executionContext,			
			bool							skipAttributeChildren) const
{
	if (skipAttributeChildren == false)
	{
		// If we should execute all children, then just call
		// executeChildren()...
		executeChildren(executionContext);
	}
	else
	{
		StylesheetExecutionContext::PushAndPopElementFrame	thePushAndPop(executionContext, this);

		for (ElemTemplateElement* node = getFirstChildElem(); node != 0; node = node->getNextSiblingElem()) 
		{
			if (node->getXSLToken() != StylesheetConstructionContext::ELEMNAME_ATTRIBUTE)
			{
				node->execute(executionContext);
			}
		}
	}
}



void
ElemElement::fixupDefaultNamespace(StylesheetExecutionContext&	executionContext) const
{
	// OK, now let's check to make sure we don't have to change the default namespace...
	const XalanDOMString* const		theCurrentDefaultNamespace =
				executionContext.getResultNamespaceForPrefix(s_emptyString);

	const XalanDOMString* const		theElementDefaultNamespace =
				getNamespacesHandler().getNamespace(s_emptyString);

	if (theCurrentDefaultNamespace != 0)
	{
		if (theElementDefaultNamespace == 0)
		{
			// There was no default namespace, so we have to turn the
			// current one off.
			executionContext.addResultAttribute(DOMServices::s_XMLNamespace, s_emptyString);
		}
		else if (equals(*theCurrentDefaultNamespace, *theElementDefaultNamespace) == false)
		{
			// There is a default namespace, but it's different from the current one,
			// so we have to change it.
			executionContext.addResultAttribute(DOMServices::s_XMLNamespace, *theElementDefaultNamespace);
		}
	}
	else
	{
		// There's no current default namespace in the result tree, but there may be
		// on for this xsl:element instance, so we have to add it, if so.
		const XalanDOMString&	theParentDefaultNamespace =
					getParentDefaultNamespace();

		if (theElementDefaultNamespace != 0)
		{
			if (length(theParentDefaultNamespace) == 0 || theParentDefaultNamespace != *theElementDefaultNamespace)
			{
				executionContext.addResultAttribute(DOMServices::s_XMLNamespace, *theElementDefaultNamespace);
			}
		}
	}
}



const XalanDOMString&
ElemElement::getParentDefaultNamespace() const
{
	const ElemTemplateElement* const	theParent =
		getParentNodeElem();

	if (theParent == 0)
	{
		return s_emptyString;
	}
	else
	{
		const XalanDOMString* const		theParentDefaultNamespace =
						theParent->getNamespacesHandler().getNamespace(s_emptyString);

		if (theParentDefaultNamespace == 0)
		{
			return s_emptyString;
		}
		else
		{
			return *theParentDefaultNamespace;
		}
	}
}



XALAN_CPP_NAMESPACE_END
