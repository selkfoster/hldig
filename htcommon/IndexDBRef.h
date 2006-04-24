//
// IndexDBRef.h
//
// IndexDBRef: Reference to an indexed document. Keeps track of all
//              information stored on the document, either by the dig 
//              or temporary search information.
//
// Part of the ht://Dig package   <http://www.htdig.org/>
// Copyright (c) 1995-2004 The ht://Dig Group
// For copyright details, see the file COPYING in your distribution
// or the GNU Library General Public License (LGPL) version 2 or later
// <http://www.gnu.org/copyleft/lgpl.html>
//
// $Id: IndexDBRef.h,v 1.1.2.3 2006/04/24 23:53:44 aarnone Exp $
//

#ifndef _IndexDBRef_h_
#define _IndexDBRef_h_

#include "htString.h"
#include "List.h"

#include <time.h>


class IndexDBRef : public Object
{
    public:
    //
    // Construction/Destruction
    //
    IndexDBRef();
    ~IndexDBRef();


    //
    // Get functions
    //
    String          DocURL()                {return URL;}
    time_t          DocTime()               {return time;}
    int             DocSig()                {return sig;} 
    int             DocHopCount()           {return hopCount;}
    int             DocBacklinks()          {return backlinks;}
    int             DocSize()               {return docSize;}
    int             DocSpiderable()         {return spiderable;}
//    List            *Descriptions()         {return &descriptions;}

    //
    // Set functions
    // 
    void        DocURL(const char *u)       {URL = u;}
    void        DocTime(time_t t)           {time = t;}
    void        DocSig(int i)               {sig = i;}
    void        DocHopCount(int i)          {hopCount = i;}
    void        DocBacklinks(int i)         {backlinks = i;} 
    void        DocSpiderable(int i)        {spiderable = i;}
    void        DocSize(int i)              {docSize = i;}
//    void        Descriptions(List &l)       {descriptions = l;}

//    void        AddDescription(const char *d, HtWordList &words);
 

    //
    // An IndexDBRef can read itself from a character string
    // and convert itself into a character string
    //
    void        Serialize(String &s);
    void        Deserialize(String &s);

    //
    // Reset Everything
    // 
    void        Clear();



    protected:
    // This is the URL of the document.
    String      URL;


    //
    // These values will be stored when serializing
    //

    // This is the time specified in the document's header
    // Usually that's the last modified time, for servers that return it.
    time_t      time;

    // This is a signature of the document. (e.g. md5sum, checksum...)
    // This is currently unused.
    long int    sig;

    // This is a count of the number of hops from start_urls to here.
    int         hopCount;

    // This is a count of the links to the document (incoming links).
    int         backlinks;

    // this is the size of the document
    int         docSize;

    // this denotes if the document is considered spiderable: 
    // AKA it is not not retrievable (certain custom documents
    // might not be spiderable)
    int         spiderable;

    // This is a list of Strings, the text of links pointing to this document.
    // (e.g. <a href="docURL">description</a>
    //List        descriptions;

};

#endif


