#pragma warning(disable:4786)

#include "../../MFCApplication1/stdafx.h"


#include <stdlib.h>


//hbf-disable warning about debug info truncated
#pragma warning(disable : 4786)


#include "../../libs/xml/expat.h"
#include "xml.h"
#include <assert.h>

/**
 ******************************************************************************
 * XmlNode implementation.
 ******************************************************************************
 */

XmlNode::~XmlNode()
{
	// Clear parent pointer from childs.
	for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
		(*it)->m_parent = 0;
	}
}

bool XmlNode::isTag( const String &tag ) const
{
	return _stricmp( tag.c_str(),m_tag.c_str()) == 0;
}

String XmlNode::getAttribute( const String &key ) const
{
	XmlAttribute a;
	a.key = key;
	XmlAttributes::const_iterator it = m_attributes.find(a);
	if (it != m_attributes.end()) {
		return it->value;
	}
	return "";
}

bool XmlNode::haveAttribute( const String &key ) const
{
	XmlAttribute a;
	a.key = key;
	XmlAttributes::const_iterator it = m_attributes.find(a);
	if (it != m_attributes.end()) {
		return true;
	}
	return false;
}

void XmlNode::setAttribute( const String &key,const String &value )
{
	XmlAttribute a;
	a.key = key;
	a.value = value;
	m_attributes.insert(a);
}

XmlNodeRef XmlNode::findChild( const String &tag ) const {
	for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
		if ((*it)->isTag(tag)) {
			return *it;
		}
	}
	return 0;
}

//! Adds new child node.
void XmlNode::addChild( const XmlNodeRef &node )
{
	assert( node != 0 );
	m_childs.push_back(node);
	node->m_parent = this;
};

//! Get XML Node child nodes.
XmlNodeRef XmlNode::getChild( unsigned int i ) const
{
	assert( i >= 0 && i < m_childs.size() );
	return m_childs[i];
}

void XmlNode::getValue( String &value ) const
{
	value = m_content;
}

void XmlNode::getValue( int &value ) const
{
	value = atoi(m_content.c_str());
}

void XmlNode::getValue( float &value ) const
{
	value = static_cast<float>(atof(m_content.c_str()));
}

void XmlNode::setValue( const String &value )
{
	m_content = value;
}

void XmlNode::setValue( int value )
{
	char str[128];
#if _MSC_VER<=1200
	itoa( value,str,10 );
#else
	_itoa_s( value,str,sizeof(str),10 );
#endif
	m_content = str;
}

void XmlNode::setValue( float value )
{
	char str[128];
#if _MSC_VER<=1200
	sprintf( str,"%f",value );
#else
	sprintf_s( str,sizeof(str),"%f",value );
#endif
	m_content = str;
}


bool XmlNode::getSubNodeValue( const String &tag,String &value ) const
{
	XmlNodeRef node = findChild( tag );
	if (node) {
		node->getValue( value );
		return true;
	}
	return false;
}
bool XmlNode::getSubNodeValue( const String &tag,int &value ) const
{
	XmlNodeRef node = findChild( tag );
	if (node) {
		node->getValue( value );
		return true;
	}
	return false;
}
bool XmlNode::getSubNodeValue( const String &tag,float &value ) const
{
	XmlNodeRef node = findChild( tag );
	if (node) {
		node->getValue( value );
		return true;
	}
	return false;
}

void XmlNode::setSubNodeValue( const String &tag,const String &value )
{
	XmlNodeRef node = findChild( tag );
	if (!node) {
		node = new XmlNode( tag );
		addChild( node );
	}
	node->setValue( value );
}
void XmlNode::setSubNodeValue( const String &tag,int value )
{
	XmlNodeRef node = findChild( tag );
	if (!node) {
		node = new XmlNode( tag );
		addChild( node );
	}
	node->setValue( value );
}
void XmlNode::setSubNodeValue( const String &tag,float value )
{
	XmlNodeRef node = findChild( tag );
	if (!node) {
		node = new XmlNode( tag );
		addChild( node );
	}
	node->setValue( value );
}

String XmlNode::getXML() const {
	String xml;

	// Begin Tag
	if (m_attributes.empty()) {
		xml = "<" + m_tag + ">";
	} else {
		xml = "<" + m_tag + " ";

		// Put attributes.
		for (XmlAttributes::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
		{
			xml += it->key+"=\""+it->value+"\"";
		}
		if (m_content.empty() && m_childs.empty()) {
			// Compact tag form.
			xml += "/>\n";
			return xml;
		}
		xml += ">";
	}

	// Put node content.
	xml += m_content;

	if (!m_childs.empty()) {
		xml += "\n";
	}

	// Put sub nodes.
	for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
		xml += (*it)->getXML();
	}

	// End tag.
	xml += "</" + m_tag + ">\n";
	return xml;
}

/**
 ******************************************************************************
 * XmlParserImp class.
 ******************************************************************************
 */
class XmlParserImp {
public:
	XmlParserImp();
//	XmlNodeRef parse( const std::vector<char> &buffer,String &errorString );
		XmlNodeRef parse( const char * buffer,int size,String &errorString );

protected:
	void	onStartElement( const char *tagName,const char **atts );
	void	onEndElement( const char *tagName );
	void	onRawData( const String& data );

	static void startElement(void *userData, const char *name, const char **atts) {
		((XmlParserImp*)userData)->onStartElement( name,atts );
	}
	static void endElement(void *userData, const char *name ) {
		((XmlParserImp*)userData)->onEndElement( name );
	}
	static void characterData( void *userData, const char *s, int len ) {
		char str[32768];
		assert( len < 32768 );
#if _MSC_VER<=1200
		strncpy( str,s,len );
#else
		strncpy_s( str,sizeof(str),s,len );
#endif
		str[len] = 0;
		((XmlParserImp*)userData)->onRawData( str );
	}

	// First node will become root node.
	std::vector<XmlNodeRef> nodeStack;
	XmlNodeRef m_root;

	XML_Parser m_parser;
};

/**
 ******************************************************************************
 * XmlParserImp
 ******************************************************************************
 */
XmlParserImp::XmlParserImp()
{
	m_root = 0;
}

void	XmlParserImp::onStartElement( const char *tagName,const char **atts )
{
	XmlNodeRef parent;
	XmlNodeRef node( tagName );

	if (!nodeStack.empty()) {
		parent = nodeStack.back();
	} else {
		m_root = node;
	}
	nodeStack.push_back(node);

	if (parent) {
		parent->addChild( node );
	}

	node->setLine( XML_GetCurrentLineNumber( (XML_Parser)m_parser ) );
	
	// Call start element callback.
	String key,value;
	int i = 0;
	while (atts[i] != 0) {
		node->setAttribute( atts[i],atts[i+1] );
		i += 2;
	}
}

void	XmlParserImp::onEndElement( const char *tagName )
{
	assert( !nodeStack.empty() );
	if (!nodeStack.empty()) {
		nodeStack.pop_back();
	}
}

void	XmlParserImp::onRawData( const String& data )
{
	assert( !nodeStack.empty() );
	if (!nodeStack.empty()) {
		XmlNode *node = nodeStack.back();
		node->setValue( data );
	}
}

//XmlNodeRef XmlParserImp::parse( const std::vector<char> &buffer,String &errorString )
XmlNodeRef XmlParserImp::parse( const char * buffer, int buffersize,String &errorString )
{
	m_parser = XML_ParserCreate(NULL);

  XML_SetUserData( m_parser, this );
  XML_SetElementHandler( m_parser, startElement,endElement );
	XML_SetCharacterDataHandler( m_parser,characterData );

	XmlNodeRef root = 0;

	/*
	int size = file->size();
	char *buf = (char*)memory::malloc( size );
	file->read( buf,size );
	if (!XML_Parse( parser,buf,size,1 ))
	{
		error( "XML Error (%s): %s at line %d\n",filename.c_str(),XML_ErrorString(XML_GetErrorCode(parser)),XML_GetCurrentLineNumber(parser) );
		return false;
	}
	memory::free( buf );

	XMLParser::parse( fileName );
	*/

//	if (XML_Parse( m_parser,buffer,buffer.size(),1 ))
		if (XML_Parse( m_parser,buffer,buffersize,1 ))
	{
		root = m_root;
	} else {
		char str[32768];
#if _MSC_VER<=1200
		sprintf( str,"XML Error: %s at line %d",XML_ErrorString(XML_GetErrorCode(m_parser)),XML_GetCurrentLineNumber(m_parser) );
#else
		sprintf_s( str,sizeof(str),"XML Error: %s at line %d",XML_ErrorString(XML_GetErrorCode(m_parser)),XML_GetCurrentLineNumber(m_parser) );
#endif
		errorString = str;
	}
	
	XML_ParserFree( m_parser );

	m_root = 0;

	return root;
}

//! Parse xml file.
XmlNodeRef XmlParser::parse( const String& fileName )
{
	m_errorString = "";
	XmlParserImp xml;
	//std::vector<char> buf;
	//String buf;
	char *buf;

	FILE *file;
#if _MSC_VER<=1200
	file = fopen( fileName.c_str(),"rb" );
	if (file) {
#else
	if (!fopen_s(&file, fileName.c_str(),"rb" )) {
#endif
		fseek( file,0,SEEK_END );
		int fileSize = ftell(file);
		fseek( file,0,SEEK_SET );

		int buffersize = fileSize;
		buf=new char[buffersize ];

		fread( buf,fileSize,1,file );
//		buf.resize( fileSize );
//		fread( buf.begin(),fileSize,1,file );
		fclose(file);

		XmlNodeRef nodeR =xml.parse( buf,buffersize ,m_errorString );
		
		delete buf;

		return nodeR;
			
	} else {
		return XmlNodeRef();
	}
}
	
//! Parse xml from memory buffer.
XmlNodeRef XmlParser::parse( const std::vector<char> &buffer )
{
	m_errorString = "";
	XmlParserImp xml;
	
	//return xml.parse( buffer,m_errorString );

	assert(0);
		XmlNodeRef nodeR;
		return nodeR;
	
};