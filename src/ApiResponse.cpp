/*
 * ApiResponse.cpp
 *
 *  Created on: Feb 18, 2012
 *      Author: Kyle
 */

#include "ApiResponse.h"
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore>
#include "json.h"

ApiResponse::ApiResponse(ApiResponseObjectFactory::ApiType targetType) {
	response = ApiResponseObjectFactory::createApiResponseObject(targetType);
	type = targetType;
}

ApiResponse::~ApiResponse() {
}

AbstractObjectBase* ApiResponse::getResponse() {
	return response;
}

ResponseMeta* ApiResponse::getMeta() {
	return meta;
}

ApiResponseObjectFactory::ApiType ApiResponse::getType() {
	return type;
}

void ApiResponse::parse(QByteArray data) {
	QString json(data);
	bool ok;
	QVariantMap resp = QtJson::Json::parse(json,ok).toMap();
	foreach(const QString &key, resp.keys()) {
		QVariantMap v = resp[key].toMap();
		if(key == "meta") {
			meta = new ResponseMeta();
			meta->parse(v);
		} else if(key == "response"){
			if(response != NULL) {
				response->parse(v);
			}
		}
	}
}
