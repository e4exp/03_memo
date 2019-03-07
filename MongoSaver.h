#pragma once


#include "stdafx.h"

//undefine min, max because mongocxx has its own min, max
#undef min
#undef max

#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/instance.hpp>
#include <mongoc/mongoc.h>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>



#include <iostream>
#include<vector>
#include <cstdint>

#include "Picker.h"
#define STR(var) #var

int exec(int argc, char *argv[]);


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


class MongoSaver {


public:
	MongoSaver();
	~MongoSaver();

	void insert_note(CString, CString);
	void update_note(CString, CString, CString id);
	void update_title(CString, CString id);
	void update_body(CString, CString id);
	int load_notes();
	void delete_note(CString id);


public:

	char* db_name = "memo";
	char* coll_name = "notes";

	mongocxx::instance instance{};
	mongocxx::client client{ mongocxx::uri{} };
	mongocxx::database db;
	mongocxx::collection coll;

	
	static const int COLUMN_NUM = 6; //all columns
	int notes_cnt;
	
	Picker picker;

	bool save_title = false;
	bool save_body = false;


	enum fields {
		_id,
		title,
		note,
		created,
		updated,
		deleted
	};



};