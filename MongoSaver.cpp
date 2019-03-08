
#include "stdafx.h"

#include "MongoSaver.h"
#include "Utils.h"


#include "picojson.h"
#include <iostream>
#include <fstream>
#include <sstream>


int exec(int argc, char *argv[]) {

	/*
	mongocxx::instance inst{};
	mongocxx::client conn{ mongocxx::uri{} };

	bsoncxx::builder::stream::document document{};

	auto collection = conn["testdb"]["testcollection"];
	document << "hello" << "world";

	collection.insert_one(document.view());
	auto cursor = collection.find({});

	for (auto&& doc : cursor) {
		std::cout << bsoncxx::to_json(doc) << std::endl;
	}
	*/
	
	return 1;
}



MongoSaver::MongoSaver() {

	//set ssl
	// If the server certificate is not signed by a well-known CA,
	// you can set a custom CA file with the `ca_file` option.
	// ssl_options.ca_file("/path/to/custom/cert.pem");

	// If you want to disable certificate verification, you
	// can set the `allow_invalid_certificates` option.
	// ssl_options.allow_invalid_certificates(true);

	//get connection string from file
	std::ifstream fin(conn_file_name);
	if (!fin) {
		AfxMessageBox("connection file cant be read");
		return;
	}
	std::stringstream strstream;
	strstream << fin.rdbuf();
	fin.close();

	//content -> string
	std::string conn_str(strstream.str());

	client_options.ssl_opts(ssl_options);
	client = mongocxx::client { mongocxx::uri{conn_str} };

	//will be created when first storing data
	db = client[db_name];
	coll = db[coll_name]; 
	
	/*
	long str_time = get_unix_time();
	bsoncxx::builder::stream::document document{};
	document << "id" << "3" 
		<<"note" << "hello world"
		<< "title" << "test"
		<< "created" << str_time
		<< "updated" << str_time
		<< "deleted" << "0"	;
	coll.insert_one(document.view());
	*/



}


MongoSaver::~MongoSaver() {

}



void MongoSaver::insert_note(CString title, CString body) {
	long str_time = get_unix_time();

	bsoncxx::builder::stream::document document{};
	document << STR(id) << "0"
		<< STR(note) << cstr_to_str(title)
		<< STR(title) << cstr_to_str(body)
		<< STR(created) << str_time
		<< STR(updated) << str_time
		<< STR(deleted) << "0";
	coll.insert_one(document.view());
}


void MongoSaver::update_note(CString title, CString body, CString id) {
	long str_time = get_unix_time();

	coll.update_one(document{} << "_id" << bsoncxx::oid(cstr_to_str(id)) << finalize,
		document{} << "$set" << open_document <<
		"title" << cstr_to_str(title) <<
		"note" << cstr_to_str(body) <<
		"updated" << str_time << close_document << finalize);

}

void MongoSaver::update_title(CString title,CString id) {
	long str_time = get_unix_time();

	coll.update_one(document{} << "_id" << bsoncxx::oid(cstr_to_str(id)) << finalize,
		document{} << "$set" << open_document <<
		"title" << cstr_to_str(title) <<
		"updated" << str_time << close_document << finalize);


}



void MongoSaver::update_body(CString body, CString id) {
	long str_time = get_unix_time();

	coll.update_one(document{} << "_id" << bsoncxx::oid(cstr_to_str(id)) << finalize,
		document{} << "$set" << open_document <<
		"note" << cstr_to_str(body) <<
		"updated" << str_time << close_document << finalize);


}


void MongoSaver::delete_note(CString id) {
	long str_time = get_unix_time();

	coll.update_one(document{} << "_id" << bsoncxx::oid(cstr_to_str(id)) << finalize,
		document{} << "$set" << open_document <<
		"deleted" << 1 << 
		"updated" << str_time << close_document << finalize);
		

}




int MongoSaver::load_notes() {


	int n_all = coll.count_documents({});
	int n_valid;

	if (n_all == 0)return 0;
	else {

		// "0" is not int. needs string.
		n_valid = coll.count_documents(document{} << "deleted" << open_document <<
			"$eq" << "0" << close_document << finalize);

		CString cst;
		cst.Format("%d", n_valid);
		//AfxMessageBox(cst);
		
		auto opts = mongocxx::options::find{};
		opts.sort(document{} << "updated" << -1 << finalize);
		mongocxx::cursor cursor = coll.find(document{} << "deleted" << open_document <<
			"$eq" << "0" << close_document << finalize, opts);

		//get whole count
		picker.whole_cnt = n_valid;

		//insert holder notes, titles, ids
		picker.holder.resize(n_valid);
		int idx_r=0;
		int i = COLUMN_NUM;
		for (auto &&doc : cursor) {//iterate by row

			//get json
			auto str = bsoncxx::to_json(doc);
			//cst = str.c_str();
			//AfxMessageBox(cst);

			//parse json into string
			picojson::value v;
			const std::string err = picojson::parse(v, str);
			if (err.empty() == false) {
				AfxMessageBox(err.c_str());
				return 0;
			}


			//store cstring to vector
			picojson::object& obj = v.get<picojson::object>();
			picker.holder[idx_r].resize(COLUMN_NUM);
			for (const auto& p : obj) { //iterate by col
				CString field = p.first.c_str();
				if (field == "id")continue;
				else {
					
					CString value;
					if (field == "_id") value = p.second.get("$oid").to_str().c_str();
					else value = p.second.to_str().c_str();

					//AfxMessageBox(field + ":" + value);


					picker.store_field(idx_r, field, value);
				}
				
			}

			idx_r++;
			
		}

	}


	return n_valid;

}



