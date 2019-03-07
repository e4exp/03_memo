
#include "stdafx.h"

#include "MongoSaver.h"
#include "picojson.h"


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
	AfxMessageBox("MongoSaver()");
		
	//will be created when first storing data
	db = client[db_name];
	coll = db[coll_name]; 
	

	/*
	bsoncxx::builder::stream::document document{};
	document << "id" << "0" 
		<<"note" << "hello world"
		<< "title" << "test"
		<< "created" << "1"
		<< "updated" << "1"
		<< "deleted" << "0"	;
	coll.insert_one(document.view());
	*/

}


MongoSaver::~MongoSaver() {

}



void MongoSaver::insert_note(CString title, CString body) {
	AfxMessageBox("insert_notes");

	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "id" << 0
		<< "note" << body
		<< "title" << title
		<< "created" << 1
		<< "updated" << 1
		<< "deleted" << 0
		<< bsoncxx::builder::stream::finalize;

	bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
		coll.insert_one(doc_value.view());
		

}


void MongoSaver::update_note(CString title, CString body, int id) {

	coll.update_one(document{} << "_id" << id << finalize,
		document{} << "$set" << open_document <<
		"title" << title <<
		"note" << body <<
		"updated" << 1 << close_document << finalize);

}

void MongoSaver::update_title(CString title, int id) {


	coll.update_one(document{} << "_id" << id << finalize,
		document{} << "$set" << open_document <<
		"title" << title <<
		"updated" << 1 << close_document << finalize);


}



void MongoSaver::update_body(CString body, int id) {

	coll.update_one(document{} << "_id" << id << finalize,
		document{} << "$set" << open_document <<
		"note" << body <<
		"updated" << 1 << close_document << finalize);


}


void MongoSaver::delete_note(int id) {
	AfxMessageBox("delete_notes");

	coll.update_one(document{} << "_id" << id << finalize,
		document{} << "$set" << open_document <<
		"deleted" << 1 << 
		"updated" << 1 << close_document << finalize);
		

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
		AfxMessageBox(cst);
		
		
		auto opts = mongocxx::options::find{};
		opts.sort(document{} << "_id" << -1 << finalize);
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
				CString value = p.second.to_str().c_str();
				//AfxMessageBox(field);

				if (p.first.c_str() == "_id")continue;
				picker.store_field(idx_r, field, value);
			}

			idx_r++;
			
		}

	}


	return n_valid;

}



