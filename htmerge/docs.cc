//
// docs.cc
//
// Indexing the "doc_db" database by id-number in "doc_index".
//
// $Id: docs.cc,v 1.15 1999/03/03 04:46:57 ghutchis Exp $
//
//

#include "htmerge.h"


//*****************************************************************************
// void convertDocs(char *doc_db, char *doc_index)
//
void
convertDocs(char *doc_db, char *doc_index)
{
    Database	*index = Database::getDatabaseInstance(DB_BTREE);
    int		document_count = 0;
    unsigned long docdb_size = 0;
    int		remove_unused = config.Boolean("remove_bad_urls");
    DocumentDB	db;
    List	*urls;

    if (index->OpenReadWrite(doc_index, 0664) == NOTOK)
    {
	reportError(form("Unable to create document index '%s'", doc_index));
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
    db.Open(doc_db);
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
	if (strlen(ref->DocHead()) == 0)
	  {
	    // For some reason, this document doesn't have an excerpt
	    // (probably because of a noindex directive, or disallowed
	    // by robots.txt or server_max_docs). Remove it
	    db.Delete(url->get());
	  }
	else if ((ref->DocState()) == Reference_noindex)
	  {
	    // This document has been marked with a noindex tag. Remove it
	    db.Delete(url->get());
	  }
	else if (remove_unused && discard_list.Exists(id))
	  {
	    // This document is not valid anymore.  Remove it
	    db.Delete(url->get());
	  }
	else
	  {
	    String coded_url(HtURLCodec::instance()->encode(ref->DocURL()));
	    index->Put(id, coded_url, strlen(coded_url));

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

    index->Close();
    delete index;
    delete urls;
    db.Close();
}


