/*
 * RequestEnvelope.cpp
 *
 *  Created on: Feb 19, 2012
 *      Author: Kyle
 */

#include "RequestEnvelope.h"

RequestEnvelope::RequestEnvelope(ApiResponseObjectFactory::ApiType type,KQOAuthRequest* apiRequest) {
	response = new ApiResponse(type);
	request = apiRequest;
}

RequestEnvelope::~RequestEnvelope() {
	delete request;
}

KQOAuthRequest* RequestEnvelope::getRequest() {
	return request;
}

void RequestEnvelope::makeRequest() {
	connect(FoursquareApi::instance()->getRequestManager(), SIGNAL(authorizedRequestReady(QByteArray,int)), this, SLOT(onRequestCallback(QByteArray,int)));
	FoursquareApi::instance()->getRequestManager()->executeAuthorizedRequest(request, 0);
}

void RequestEnvelope::onRequestCallback(QByteArray resp, int id) {
	disconnect(FoursquareApi::instance()->getRequestManager(), SIGNAL(authorizedRequestReady(QByteArray,int)), this, SLOT(onRequestCallback(QByteArray, int)));
	response->parse(resp);
	qDebug() << resp;
	if(response != NULL && response->getMeta() == NULL) {
		return;
	}
	if(response->getMeta()->getStatus() == 200 || response->getMeta()->getStatus() == 201) {
		emit requestComplete(response->getResponse());
	} else if(response->getType() == ApiResponseObjectFactory::Empty){
		emit requestComplete(NULL);
	} else {
		emit requestError(response->getMeta());
	}
}
