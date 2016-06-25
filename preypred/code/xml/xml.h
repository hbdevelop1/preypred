#ifndef __XML_HEADER__
#define __XML_HEADER__

#include <string>
#include <vector>
#include <set>

typedef std::string String;


/**

	This is wrapper arround expat library to provide DOM type of access for xml.
	Do not use XmlNode class directly instead always use XmlNodeRef wrapper that
	takes care of memory managment issues.

	Usage Example:
	-------------------------------------------------------
	void testXml()
	{
		XmlParser xml;
		XmlNodeRef root = xml.parse( "test.xml" );

		if (root) {
			for (int i = 0; i < root->getChildCount(); i++) {
				XmlNodeRef child = root->getChild(i);
				if (child->isTag("world")) {
					if (child->getAttribute("name") == "blah") {
					}
				}
			}
		}
	};
*/

/**
 ******************************************************************************
 * XmlAttribute class
 ******************************************************************************
 */

class XmlAttribute {
public:
	String key;
	String value;
	
	XmlAttribute() {}
	XmlAttribute( const XmlAttribute &attr ) { *this = attr; }
	XmlAttribute& operator=( const XmlAttribute &attr ) {
		key = attr.key;
		value = attr.value;
		return *this;
	}
	bool operator<( const XmlAttribute &attr ) const {
		return _stricmp( key.c_str(),attr.key.c_str() ) < 0;
	}
	bool operator==( const XmlAttribute &attr ) const {
		return _stricmp( key.c_str(),attr.key.c_str() ) == 0;
	}
	bool operator!=( const XmlAttribute &attr ) const {
		return !(*this == attr);
	}
};


class XmlNode;

//! Xml node attributes class.
typedef std::set<XmlAttribute>	XmlAttributes;

/**
 ******************************************************************************
 * XmlNodeRef, wrapper class implementing reference counting for XmlNode.
 ******************************************************************************
 */
class XmlNodeRef {
private:
  XmlNode* p;
public:
  XmlNodeRef() : p(NULL) {}
	XmlNodeRef( int Null ) : p(NULL) {}
  XmlNodeRef( XmlNode* p_ );
  XmlNodeRef( const XmlNodeRef &p_ );
	explicit XmlNodeRef( const String &tag );
  ~XmlNodeRef();
	
  operator XmlNode*() const { return p; }
  operator const XmlNode*() const { return p; }
  XmlNode& operator*() const { return *p; }
  XmlNode* operator->(void) const { return p; }
  
  XmlNodeRef&  operator=( XmlNode* newp );
	XmlNodeRef&  operator=( const XmlNodeRef &newp );
	
  operator bool() const { return p != NULL; };
	bool operator !() const { return p == NULL; };
	
 	// Misc compare functions.
  bool  operator == ( const XmlNode* p2 ) const { return p == p2; };
  bool  operator != ( const XmlNode* p2 ) const { return p != p2; };
  bool  operator <  ( const XmlNode* p2 ) const { return p < p2; };
  bool  operator >  ( const XmlNode* p2 ) const { return p > p2; };
	
	friend bool operator == ( const XmlNodeRef &p1,int null );
  friend bool operator != ( const XmlNodeRef &p1,int null );
	friend bool operator == ( int null,const XmlNodeRef &p1 );
  friend bool operator != ( int null,const XmlNodeRef &p1 );
};

/************************************************************************/
/* XmlParser class, Parse xml and return root xml node if success.      */
/************************************************************************/
class XmlParser
{
public:
	//! Parse xml file.
	XmlNodeRef parse( const String& fileName );
	
	//! Parse xml from memory buffer.
	XmlNodeRef parse( const std::vector<char> &buffer );

	String getErrorString() const { return m_errorString; }
private:
	String m_errorString;
};

/**
 ******************************************************************************
 * XmlNode class
 * Never use XmlNode directly instead use reference counted XmlNodeRef.
 ******************************************************************************
 */

class XmlNode {
public:
	//! Non virtual destructor, XmlNode cannot be used as a base class.
	~XmlNode();

	//! Get XML node tag.
	const String& getTag() const { return m_tag; };

	//! Return true if givven tag equal to node tag.
	bool isTag( const String &tag ) const;

	//! Get XML Node attributes.
	const XmlAttributes&	getAttributes() const { return m_attributes; };
	//! Get XML Node attribute for specified key.
	String getAttribute( const String &key ) const;
	//! Check if attributes with specified key exist.
	bool haveAttribute( const String &key ) const;

	//! Set new XML Node attribute (or override attribute with same key).
	void setAttribute( const String &key,const String &value );
	
	//! Adds new child node.
	void addChild( const XmlNodeRef &node );

	//! Get number of child XML nodes.
	int	getChildCount() const { return m_childs.size(); };
	
	//! Get XML Node child nodes.
	XmlNodeRef getChild( unsigned int i ) const;

	//! Find node with specified tag.
	XmlNodeRef findChild( const String &tag ) const;

	//! Get parent XML node.
	XmlNodeRef	getParent() const { return m_parent; }

	//! Returns content of this node.
	const String& getContent() const { return m_content; };

	//! Returns line number for XML tag.
	int getLine() const { return m_line; };
	//! Set line number in xml.
	void setLine( int line ) { m_line = line; };

	//! Returns XML of this node and sub nodes.
	String getXML() const;

	//! Get value of node.
	void getValue( String &value ) const;
	void getValue( int &value ) const;
	void getValue( float &value ) const;
	void getValue( bool &value ) const { int v; getValue(v); value = (v != 0); }

	//! Set value of node.
	void setValue( const String &value );
	void setValue( int value );
	void setValue( float value );

	//! Get value of sub node.
	bool getSubNodeValue( const String &tag,String &value ) const;
	bool getSubNodeValue( const String &tag,int &value ) const;
	bool getSubNodeValue( const String &tag,float &value ) const;

	//! Set value of sub node (Create sub node if neccesarry).
	void setSubNodeValue( const String &tag,const String &value );
	void setSubNodeValue( const String &tag,int value );
	void setSubNodeValue( const String &tag,float value );

private:
	//! The only ctor and private, protect us from deriviation.
	XmlNode( const String &tag ) {
		m_tag = tag;
		m_parent = 0;
		m_refCount = 0;
	}

	//! Reference counting.
	void addRef() { m_refCount++; };
	//! When ref count reach zero XML node dies.
	void release() { if (--m_refCount <= 0) delete this; };

	//! Ref count itself, its zeroed on node creation.
	int m_refCount;

	//! Line in XML file where this node firstly appeared (usefull for debuggin).
	int m_line;
	//! Tag of XML node.
	String m_tag;

	//! Content of XML node.
	String m_content;
	//! Parent XML node.
	XmlNode *m_parent;
	//! Next XML node in same hierarchy level.
	
	typedef std::vector<XmlNodeRef>	XmlNodes;
	XmlNodes m_childs;
	//! Xml node attributes.
	XmlAttributes m_attributes;

	//! Lets be friendly to him.
	friend class XmlNodeRef;
};


///////////////////////////////////////////////////////////////////////////////
// Inline Implementation of XmlNodeRef
inline XmlNodeRef::XmlNodeRef( const String &tag )
{
	p = new XmlNode( tag );
	p->addRef();
}

inline XmlNodeRef::XmlNodeRef( XmlNode* p_ ) : p(p_)
{
	if (p) p->addRef();
}

inline XmlNodeRef::XmlNodeRef( const XmlNodeRef &p_ ) : p(p_.p)
{
	if (p) p->addRef();
}

inline XmlNodeRef::~XmlNodeRef()
{
	if (p) p->release();
}

inline XmlNodeRef&  XmlNodeRef::operator=( XmlNode* newp )
{
	if (newp) newp->addRef();
	if (p) p->release();
	p = newp;
	return *this;
}

inline XmlNodeRef&  XmlNodeRef::operator=( const XmlNodeRef &newp )
{
	if (newp.p) newp.p->addRef();
	if (p) p->release();
	p = newp.p;
	return *this;
}

inline bool operator == ( const XmlNodeRef &p1,int null )	{
	return p1.p == 0;
}

inline bool operator != ( const XmlNodeRef &p1,int null )	{
	return p1.p != 0;
}

inline bool operator == ( int null,const XmlNodeRef &p1 )	{
	return p1.p == 0;
}

inline bool operator != ( int null,const XmlNodeRef &p1 )	{
	return p1.p != 0;
}

#endif // __XML_HEADER__