/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#if !defined(UNIMPLEMENTEDELEMENT_HEADER_GUARD_1357924680)
#define UNIMPLEMENTEDELEMENT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <DOMSupport/DOMSupportDefinitions.hpp>



#include <dom/DOM_Element.hpp>

#include <dom/ElementImpl.hpp>



class XALAN_DOMSUPPORT_EXPORT UnimplementedElement : public ElementImpl
{
public:

	/**
	 * Constructor.
	 */
	UnimplementedElement(DocumentImpl*		ownerDoc);

	UnimplementedElement(
			const UnimplementedElement&	theOther,
			bool						deep = false);

	virtual
	~UnimplementedElement();

	// These interfaces are inherited from RefCountedImpl...

	virtual void
	referenced();

	virtual void
	unreferenced();

	// These interfaces are inherited from NodeListImpl...

	virtual NodeImpl*
	item(unsigned long	index);

	virtual unsigned int
	getLength();

	// These interfaces are inherited from NodeImpl...

    virtual bool
	isElementImpl();

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual NodeImpl*
#else
	virtual UnimplementedElement*
#endif
	cloneNode(bool deep) = 0;

	virtual NodeImpl*
	appendChild(NodeImpl*	newChild);

	virtual void
	changed();

	virtual NamedNodeMapImpl*
	getAttributes();

	virtual NodeListImpl*
	getChildNodes();

	virtual NodeImpl*
	getFirstChild();

	virtual NodeImpl*
	getLastChild();

	virtual NodeImpl*
	getNextSibling();

	virtual DOMString
	getNodeName();

	virtual short
	getNodeType();

	virtual DOMString
	getNodeValue();

	virtual DocumentImpl*
	getOwnerDocument();

	virtual NodeImpl* 
	getParentNode();

	virtual NodeImpl* 
	getPreviousSibling();

	virtual void*
	getUserData();

	virtual bool
	hasChildNodes();

	virtual NodeImpl*
	insertBefore(NodeImpl *newChild, NodeImpl *refChild);

	virtual NodeImpl*
	removeChild(NodeImpl *oldChild);

	virtual NodeImpl*
	replaceChild(NodeImpl *newChild, NodeImpl *oldChild);

	virtual void
	setNodeValue(const DOMString&	value);

	virtual void
	setReadOnly(bool readOnly, bool deep);

	virtual void
	setUserData(void *value);

	virtual DOMString
	toString();

	// These interfaces are inherited from ElementImpl...

    virtual DOMString
	getAttribute(const DOMString&	name);

    virtual AttrImpl*
	getAttributeNode(const DOMString&	name);

    virtual DeepNodeListImpl*
	getElementsByTagName(const DOMString&	tagname);

    virtual DOMString
	getTagName();

    virtual void
	normalize();

    virtual void
	removeAttribute(const DOMString&	name);

    virtual AttrImpl*
	removeAttributeNode(AttrImpl*	oldAttr);

    virtual void
	setAttribute(
			const DOMString&	name,
			const DOMString&	value);

    virtual AttrImpl*
	setAttributeNode(AttrImpl*	newAttr);
};



class XALAN_DOMSUPPORT_EXPORT DOM_UnimplementedElement : public DOM_Element
{
public:

	/**
	 * Constructors.
	 */
	DOM_UnimplementedElement(UnimplementedElement*	theElement);

	DOM_UnimplementedElement();

	DOM_UnimplementedElement(const DOM_UnimplementedElement&	theElement);

	/**
	 * Destructor.
	 */
	~DOM_UnimplementedElement();

	/**
	 * Assignment operator.
	 *
	 * @param other The source to be assigned.
	 */
	DOM_UnimplementedElement&
	operator=(const DOM_UnimplementedElement&	other)
	{
		DOM_Element::operator=(other);

		return *this;
	}

	/**
	 * Assignment operator.  This overloaded variant is provided for
	 *	 the sole purpose of setting a DOM_Node reference variable to
	 *	 zero.	Nulling out a reference variable in this way will decrement
	 *	 the reference count on the underlying Node object that the variable
	 *	 formerly referenced.	This effect is normally obtained when reference
	 *	 variable goes out of scope, but zeroing them can be useful for
	 *	 global instances, or for local instances that will remain in scope
	 *	 for an extended time,	when the storage belonging to the underlying
	 *	 node needs to be reclaimed.
	 *
	 * @param val.	Only a value of 0, or null, is allowed.
	 */
	DOM_UnimplementedElement&
	operator=(const DOM_NullPtr*	val)
	{
		DOM_Element::operator=(val);

		return *this;
	}

	/**
	 * The equality operator.  This compares to references to nodes, and
	 * returns true if they both refer to the same underlying node.  It
	 * is exactly analogous to Java's operator ==  on object reference
	 * variables.  This operator can not be used to compare the values
	 * of two different nodes in the document tree.
	 *
	 * @param other The object reference with which <code>this</code> object is compared
	 * @returns True if both <code>DOM_Node</code>s refer to the same
	 *	actual node, or are both null; return false otherwise.
	 */
	bool
	operator==(const DOM_Node&	other) const
	{
		return DOM_Node::operator==(other);
	}

	/**
	 *	Compare with a pointer.  Intended only to allow a convenient
	 *	  comparison with null.
	 *
	 */
	bool
	operator==(const DOM_NullPtr*	other) const
	{
		return DOM_Node::operator==(other);
	}

	/**
	 * The inequality operator.  See operator ==.
	 *
	 */
	bool
	operator!=(const DOM_Node&	other) const
	{
		return DOM_Node::operator!=(other);
	}

	/**
	 *	Compare with a pointer.  Intended only to allow a convenient
	 *	  comparison with null.
	 *
	 */
	bool
	operator!=(const DOM_NullPtr*	other) const
	{
		return DOM_Node::operator!=(other);
	}

protected:

private:

	// Not implemented...

	// I think it's dangerous to allow this conversion...
	DOM_UnimplementedElement(const DOM_Node&	theNode);
};



#endif	// UNIMPLEMENTEDELEMENT_HEADER_GUARD_1357924680
