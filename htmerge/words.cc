//
// words.cc
//
// words: Remove words from documents that have been purged by the docs code.
//
// Part of the ht://Dig package   <http://www.htdig.org/>
// Copyright (c) 1999 The ht://Dig Group
// For copyright details, see the file COPYING in your distribution
// or the GNU Public License version 2 or later
// <http://www.gnu.org/copyleft/gpl.html>
//
// $Id: words.cc,v 1.19 1999/09/28 16:18:14 loic Exp $
//

#include "htmerge.h"
#include "HtPack.h"

#include <errno.h>

//
// Callback data dedicated to Dump and dump_word communication
//
class DeleteWordData : public Object
{
public:
  DeleteWordData(const Dictionary& discard_arg) : discard(discard_arg) { deleted = remains = 0; }

  const Dictionary& discard;
  int deleted;
  int remains;
};

//*****************************************************************************
//
//
static int delete_word(WordList *words, const WordReference *word, Object &data)
{
  DeleteWordData& d = (DeleteWordData&)data;
  static String docIDStr;

  docIDStr = 0;
  docIDStr << word->DocID();

  if(d.discard.Exists(docIDStr)) {
    if(words->Delete(*word) != 1) {
      cerr << "htmerge: deletion of " << *word << " failed " << strerror(errno) << "\n";
      return NOTOK;
    }
    if (verbose)
      {
	cout << "htmerge: Discarding " << *word << "\n";
	cout.flush();
      }
    d.deleted++;
  } else {
    d.remains++;
  }

  return OK;
}

//*****************************************************************************
// void mergeWords()
//
void mergeWords()
{
  WordList		words(config);
  DeleteWordData	data(discard_list); 

  words.Open(config["word_db"], O_RDWR);

  (void)words.Walk(WordReference(), HTDIG_WORDLIST_WALK, delete_word, data);
  
  words.Close();

  if (verbose)
    cout << "\n";
  if (stats)
    {
      cout << "htmerge: Total word count: " 
	   << data.remains << endl;
      cout << "htmerge: Total deleted words: " << data.deleted << endl;
    }

}


