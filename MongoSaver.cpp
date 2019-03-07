
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
		<< "_id" <<""
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


	int n = coll.count_documents({});
	if (n == 0)return n;
	else {


		bsoncxx::builder::stream::document document{};
		document << "deleted" << open_document <<
			"$eq" << 0 << close_document << finalize;
		int n = coll.count_documents(document.view());
		
		
		//query.Format("SELECT id, title, note, created, updated, deleted FROM notes WHERE deleted=0 ORDER BY id DESC;");
		auto order = bsoncxx::builder::stream::document{} << "_id" << -1 << finalize;
		auto opts = mongocxx::options::find{};
		opts.sort(order.view());
		mongocxx::cursor cursor = coll.find(document.view(),opts);

		//get whole count
		picker.whole_cnt = n;

		//insert holder notes, titles, ids
		picker.holder.resize(n);
		int idx_r=0;
		int i = COLUMN_NUM;
		for (auto &&doc : cursor) {//iterate by row

			//get json
			auto str = bsoncxx::to_json(doc);

			//parse json into string
			picojson::value v;
			const std::string err = picojson::parse(v, str);
			if (err.empty() == false) {
				AfxMessageBox( err.c_str());
				return 0;
			}


			//store cstring to vector
			picojson::object& obj = v.get<picojson::object>();

			/*
			CString cstr;
			cstr=obj["id"].get<std::string>().c_str();
			cstr+=obj["note"].get <std::string>().c_str();
			cstr += obj["title"].get<std::string>().c_str();
			cstr += obj["created"].get<std::string>().c_str();
			cstr += obj["updated"].get<std::string>().c_str();
			cstr += obj["deleted"].get<std::string>().c_str();
			//AfxMessageBox(cstr);
			*/

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

	return n;

}



