//
// docs.cc
//
// Do sanity checking in "doc_db", remove insane documents.
//
// $Id: docs.cc,v 1.17 1999/06/01 01:55:30 ghutchis Exp $
//
//

#include "htmerge.h"


//*****************************************************************************
// void convertDocs(char *doc_db, char *doc_index, char *doc_excerpt)
//
void
convertDocs(char *doc_db, char *doc_index, char *doc_excerpt)
{
    int		document_count = 0;
    unsigned long docdb_size = 0;
    int		remove_unused = config.Boolean("remove_bad_urls");
    DocumentDB	db;
    List	*urls;

    if (access(doc_index, R_OK) < 0)
    {
	reportError(form("Unable to open document index '%s'", doc_index));
    }
    if (access(doc_excerpt, R_OK) < 0)
    {
	reportError(form("Unable to open document excerpts '%s'", doc_excerpt));
    }
    if (access(doc_db, R_OK) < 0)
    {
	reportError(form("Unable to open document database '%s'", doc_db));
    }

    // Check "uncompressed"/"uncoded" urls at the price of time
    // (extra DB probes).
    db.SetCompatibility(config.Boolean("uncoded_db_compatible", 1));

    //
    // Start the conversion by going through all the URLs that are in
    // the document database
    //
    db.Open(doc_db, doc_index, doc_excerpt);
    urls = db.URLs();
	
    urls->Start_Get();
    String		*url;
    String		id;
    while ((url = (String *) urls->Get_Next()))
    {
	DocumentRef	*ref = db[url->get()];

	// moet eigenlijk wat tussen, maar heb ik niet gedaan....
	// (Translation Dutch -> English)
	// something should be inserted here but I didn't do that

	if (!ref)
	    continue;
	id = 0;
	id << ref->DocID();
	//	if (strlen(ref->DocHead()) == 0)
	//	  {
	//	    // For some reason, this document doesn't have an excerpt
	//	    // (probably because of a noindex directive, or disallowed
	//	    // by robots.txt or server_max_docs). Remove it
	//	    db.Delete(ref->DocID());
	//            if (verbose)
	//              cout << "Deleted, no excerpt: " << id.get() << "/"
	//                   << url->get() << endl;
	//	  }
	if ((ref->DocState()) == Reference_noindex)
	  {
	    // This document has been marked with a noindex tag. Remove it
	    db.Delete(ref->DocID());
            if (verbose)
              cout << "Deleted, noindex: " << id.get() << "/"
                   << url->get() << endl;
	  }
	else if (remove_unused && discard_list.Exists(id))
	  {
	    // This document is not valid anymore.  Remove it
	    db.Delete(ref->DocID());
            if (verbose)
              cout << "Deleted, invalid: " << id.get() << "/"
                   << url->get() << endl;
	  }
	else
	  {
            if (verbose > 1)
              cout << "" << id.get() << "/" << url->get() << endl;

	    document_count++;
	    docdb_size += ref->DocSize();
	    if (verbose && document_count % 10 == 0)
	    {
		cout << "htmerge: " << document_count << '\n';
		cout.flush();
	    }
	  }
        delete ref;
    }
    if (verbose)
	cout << "\n";
    if (stats)
      {
	cout << "htmerge: Total documents: " << document_count << endl;
	cout << "htmerge: Total doc db size (in K): ";
	cout << docdb_size / 1024 << endl;
      }

    delete urls;
    db.Close();
}


