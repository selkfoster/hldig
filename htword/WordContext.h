//
// WordContext.h
//
//
// Part of the ht://Dig package   <http://www.htdig.org/>
// Copyright (c) 1999 The ht://Dig Group
// For copyright details, see the file COPYING in your distribution
// or the GNU Public License version 2 or later
// <http://www.gnu.org/copyleft/gpl.html>
//
// $Id: WordContext.h,v 1.1.2.1 2000/01/03 10:02:06 bosc Exp $
//

#ifndef _WordContext_h_
#define _WordContext_h_
// this is an atempt to group the global parameters that were
// running around htword. 
// it's temporary until we find a better solution

#include"Configuration.h"

class WordType      ;
class WordKeyInfo   ;
class WordRecordInfo;

class WordContext
{
    friend WordType;
    friend WordKeyInfo;
    friend WordRecordInfo;
public:
    static WordType       *word_type_default;
private:
    static WordKeyInfo    *key_info;
    static WordRecordInfo *record_info;

public:
    static inline WordType       *Get_word_type_default(){return word_type_default;}
    static void Initialize(const Configuration &config);
};

#endif // _WordContext_h_
