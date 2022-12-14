2022/12/14 12:14:43 STARTUP Redis server: tcp://127.0.0.1:6379
2022/12/14 12:14:43 STARTUP ElasticSearch server: http://172.22.12.49:9200
2022/12/14 12:14:43 STARTUP ES view server: http://172.22.114.129:63300/view/view
2022/12/14 12:14:43 STARTUP CouchDB server: http://wolf_staging:******@172.22.10.201:5984
/**
 * Central Index
 *
 * The Central Index C Library
 *
 * @package 	CentralIndex
 * @author  	Glynn Bird
 * @link    	http://centralindex.com
 * @since   	Version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 1024) /* 256kB */

const char * API_URL = "http://api.centralindex.com/v1";

char  API_KEY[1000]="";

int DEBUG_MODE = 0;


struct write_result {
	char *data;
	int pos;
};

struct put_data
{
	char *data;
	size_t len;
};


static size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream) {
	
	struct write_result *result = (struct write_result *)stream;
	
	/* Will we overflow on this write? */
	if(result->pos + size * nmemb >= BUFFER_SIZE - 1) {
		fprintf(stderr, "curl error: too small buffer\n");
		return 0;
	}
	
	/* Copy curl's stream buffer into our own buffer */
	memcpy(result->data + result->pos, ptr, size * nmemb);
	
	/* Advance the position */
	result->pos += size * nmemb;
	
	return size * nmemb;
}

static size_t curl_put( void *ptr, size_t size, size_t nmemb, void *stream ) {
	
	struct put_data *userdata = (struct put_data *)stream;
	
	size_t curl_size = nmemb * size;
	size_t to_copy = (userdata->len < curl_size) ? userdata->len : curl_size;
	memcpy(ptr, userdata->data, to_copy);
	userdata->len -= to_copy;
	userdata->data += to_copy;
	return to_copy;	
}

void set_api_key ( char * apikey) {
	strcpy(API_KEY,apikey);    	
}

/* 1 = DEBUG ON  0 = DEBUG OFF */

void set_debug_mode (int debug ) {
	
    DEBUG_MODE = debug;
}

char * doCurl (char * method, char * path, char * params) {
	
	
	CURL *curl;
	CURLcode res;
	
	char *data;
	
	char *retval=NULL;
	
	
	
	/* Create the write buffer */
	data = malloc(BUFFER_SIZE);
	if (! data)
		fprintf(stderr, "Error allocating %d bytes.\n", BUFFER_SIZE);
	
	struct write_result write_result = {
		.data = data,
		.pos = 0
	};
	
	struct put_data put_data = {
		.data = params,
		.len = 0
	};
	
	char * url = NULL;
    asprintf(&url, "%s%s", API_URL,path);	
	curl = curl_easy_init();
	
	if(DEBUG_MODE != 0){
        printf("METHOD = %s\n",method);
        printf("URL = %s\n",url);
		printf("Data: %s\n",params);
	 }	
			   
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(!strcmp(method,"GET")) {				
		curl_easy_setopt(curl, CURLOPT_POST, 0);
		asprintf(&url, "%s%s", url,params);
		
	} else if (!strcmp(method,"PUT")) {
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READDATA, &put_data);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION,curl_put);
		
	} else if (!strcmp(method,"DELETE")) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	 	
	} else {
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,params);
		
	} 
		
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);	
	
	res = curl_easy_perform(curl);
	
	if(DEBUG_MODE != 0){
		printf("OUTPUT: %s\n",write_result.data);
	}		
	
 	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    } else {
		retval = write_result.data;
	}
	
	/* always cleanup */ 
	curl_easy_cleanup(curl);
	
		
	return retval;
	
	
	
	
}


  /**
   * With a 192 id get remote entity data
   *
   *  @param oneninetwo_id
   *  @return - the data from the api
  */
  char * GET192Get( char *oneninetwo_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"oneninetwo_id=");
    p = curl_easy_escape(curl,oneninetwo_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/192/get",params);
  }


  /**
   * Get the activity from the collection
   *
   *  @param type - The activity type: add, claim, special offer, image, video, description, testimonial
   *  @param country - The country to filter by
   *  @param latitude_1 - The latitude_1 to filter by
   *  @param longitude_1 - The longitude_1 to filter by
   *  @param latitude_2 - The latitude_2 to filter by
   *  @param longitude_2 - The longitude_2 to filter by
   *  @param number_results - The number_results to filter by
   *  @param unique_action - Return only the most recent instance of this action?
   *  @return - the data from the api
  */
  char * GETActivity_stream( char *type, char *country, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *number_results, char *unique_action) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_1=");
    p = curl_easy_escape(curl,latitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_1=");
    p = curl_easy_escape(curl,longitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_2=");
    p = curl_easy_escape(curl,latitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_2=");
    p = curl_easy_escape(curl,longitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"number_results=");
    p = curl_easy_escape(curl,number_results,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"unique_action=");
    p = curl_easy_escape(curl,unique_action,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/activity_stream",params);
  }


  /**
   * When we get some activity make a record of it
   *
   *  @param entity_id - The entity to pull
   *  @param entity_name - The entity name this entry refers to
   *  @param type - The activity type.
   *  @param country - The country for the activity
   *  @param longitude - The longitude for teh activity
   *  @param latitude - The latitude for teh activity
   *  @return - the data from the api
  */
  char * POSTActivity_stream( char *entity_id, char *entity_name, char *type, char *country, char *longitude, char *latitude) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_name=");
    p = curl_easy_escape(curl,entity_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/activity_stream",params);
  }


  /**
   * Get all entities in which live ads have the matched reseller_masheryid.
   *
   *  @param country
   *  @param reseller_masheryid
   *  @param name_only - If true the query result contains entity name only; otherwise, the entity object.
   *  @param name_match - Filter the result in which the name contains the given text.
   *  @param skip
   *  @param take - Set 0 to get all result. However, if name_only=false, only 100 objects at most will be returned to prevent oversized response body.
   *  @return - the data from the api
  */
  char * GETAdvertiserBy_reseller_masheryid( char *country, char *reseller_masheryid, char *name_only, char *name_match, char *skip, char *take) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_masheryid=");
    p = curl_easy_escape(curl,reseller_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name_only=");
    p = curl_easy_escape(curl,name_only,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name_match=");
    p = curl_easy_escape(curl,name_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip=");
    p = curl_easy_escape(curl,skip,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"take=");
    p = curl_easy_escape(curl,take,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/advertiser/by_reseller_masheryid",params);
  }


  /**
   * Get all advertisers that have been updated from a give date for a given reseller
   *
   *  @param from_date
   *  @param country
   *  @return - the data from the api
  */
  char * GETAdvertiserUpdated( char *from_date, char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/advertiser/updated",params);
  }


  /**
   * Get all advertisers that have been updated from a give date for a given publisher
   *
   *  @param publisher_id
   *  @param from_date
   *  @param country
   *  @return - the data from the api
  */
  char * GETAdvertiserUpdatedBy_publisher( char *publisher_id, char *from_date, char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/advertiser/updated/by_publisher",params);
  }


  /**
   * Check that the advertiser has a premium inventory
   *
   *  @param type
   *  @param category_id - The category of the advertiser
   *  @param location_id - The location of the advertiser
   *  @param publisher_id - The publisher of the advertiser
   *  @return - the data from the api
  */
  char * GETAdvertisersPremiumInventorycheck( char *type, char *category_id, char *location_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/advertisers/premium/inventorycheck",params);
  }


  /**
   * Delete an association
   *
   *  @param association_id
   *  @return - the data from the api
  */
  char * DELETEAssociation( char *association_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"association_id=");
    p = curl_easy_escape(curl,association_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/association",params);
  }


  /**
   * Fetch an association
   *
   *  @param association_id
   *  @return - the data from the api
  */
  char * GETAssociation( char *association_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"association_id=");
    p = curl_easy_escape(curl,association_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/association",params);
  }


  /**
   * Will create a new association or update an existing one
   *
   *  @param association_id
   *  @param association_name
   *  @param association_url
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTAssociation( char *association_id, char *association_name, char *association_url, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"association_id=");
    p = curl_easy_escape(curl,association_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"association_name=");
    p = curl_easy_escape(curl,association_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"association_url=");
    p = curl_easy_escape(curl,association_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/association",params);
  }


  /**
   * The search matches a category name on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @param mapped_to_partner - Only return CI categories that have a partner mapping
   *  @return - the data from the api
  */
  char * GETAutocompleteCategory( char *str, char *language, char *mapped_to_partner) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"str=");
    p = curl_easy_escape(curl,str,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapped_to_partner=");
    p = curl_easy_escape(curl,mapped_to_partner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/autocomplete/category",params);
  }


  /**
   * The search matches a category name and ID on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @param mapped_to_partner - Only return CI categories that have a partner mapping
   *  @return - the data from the api
  */
  char * GETAutocompleteCategoryId( char *str, char *language, char *mapped_to_partner) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"str=");
    p = curl_easy_escape(curl,str,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapped_to_partner=");
    p = curl_easy_escape(curl,mapped_to_partner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/autocomplete/category/id",params);
  }


  /**
   * The search matches a category name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * GETAutocompleteKeyword( char *str, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"str=");
    p = curl_easy_escape(curl,str,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/autocomplete/keyword",params);
  }


  /**
   * The search matches a location name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Dub e.g. dub
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * GETAutocompleteLocation( char *str, char *country, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"str=");
    p = curl_easy_escape(curl,str,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/autocomplete/location",params);
  }


  /**
   * The search matches a location name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Middle e.g. dub
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param resolution
   *  @return - the data from the api
  */
  char * GETAutocompleteLocationBy_resolution( char *str, char *country, char *resolution) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"str=");
    p = curl_easy_escape(curl,str,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"resolution=");
    p = curl_easy_escape(curl,resolution,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/autocomplete/location/by_resolution",params);
  }


  /**
   * Create a new business entity with all it's objects
   *
   *  @param name
   *  @param status
   *  @param building_number
   *  @param branch_name
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param province
   *  @param postcode
   *  @param country
   *  @param latitude
   *  @param longitude
   *  @param timezone
   *  @param telephone_number
   *  @param allow_no_address
   *  @param allow_no_phone
   *  @param additional_telephone_number
   *  @param email
   *  @param website
   *  @param payment_types - Payment types separated by comma
   *  @param tags - Tags separated by comma
   *  @param category_id
   *  @param category_type
   *  @param featured_message_text - Featured message content
   *  @param featured_message_url - Featured message URL
   *  @param do_not_display
   *  @param orderonline
   *  @param delivers
   *  @param referrer_url
   *  @param referrer_name
   *  @param destructive
   *  @param delete_mode - The type of object contribution deletion
   *  @param master_entity_id - The entity you want this data to go to
   *  @param no_merge_on_error - If true, data duplication error will be returned when a matched entity is found. If false, such error is suppressed and data is merged into the matched entity.
   *  @return - the data from the api
  */
  char * PUTBusiness( char *name, char *status, char *building_number, char *branch_name, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *allow_no_address, char *allow_no_phone, char *additional_telephone_number, char *email, char *website, char *payment_types, char *tags, char *category_id, char *category_type, char *featured_message_text, char *featured_message_url, char *do_not_display, char *orderonline, char *delivers, char *referrer_url, char *referrer_name, char *destructive, char *delete_mode, char *master_entity_id, char *no_merge_on_error) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"status=");
    p = curl_easy_escape(curl,status,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"building_number=");
    p = curl_easy_escape(curl,building_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"branch_name=");
    p = curl_easy_escape(curl,branch_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address1=");
    p = curl_easy_escape(curl,address1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address2=");
    p = curl_easy_escape(curl,address2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address3=");
    p = curl_easy_escape(curl,address3,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"district=");
    p = curl_easy_escape(curl,district,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"town=");
    p = curl_easy_escape(curl,town,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"county=");
    p = curl_easy_escape(curl,county,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"province=");
    p = curl_easy_escape(curl,province,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcode=");
    p = curl_easy_escape(curl,postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"timezone=");
    p = curl_easy_escape(curl,timezone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"telephone_number=");
    p = curl_easy_escape(curl,telephone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"allow_no_address=");
    p = curl_easy_escape(curl,allow_no_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"allow_no_phone=");
    p = curl_easy_escape(curl,allow_no_phone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"additional_telephone_number=");
    p = curl_easy_escape(curl,additional_telephone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"website=");
    p = curl_easy_escape(curl,website,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_types=");
    p = curl_easy_escape(curl,payment_types,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tags=");
    p = curl_easy_escape(curl,tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_type=");
    p = curl_easy_escape(curl,category_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"featured_message_text=");
    p = curl_easy_escape(curl,featured_message_text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"featured_message_url=");
    p = curl_easy_escape(curl,featured_message_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"do_not_display=");
    p = curl_easy_escape(curl,do_not_display,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_url=");
    p = curl_easy_escape(curl,referrer_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_name=");
    p = curl_easy_escape(curl,referrer_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delete_mode=");
    p = curl_easy_escape(curl,delete_mode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"master_entity_id=");
    p = curl_easy_escape(curl,master_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"no_merge_on_error=");
    p = curl_easy_escape(curl,no_merge_on_error,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/business",params);
  }


  /**
   * Create entity via JSON
   *
   *  @param json - Business JSON
   *  @param country - The country to fetch results for e.g. gb
   *  @param timezone
   *  @param master_entity_id - The entity you want this data to go to
   *  @param allow_no_address
   *  @param allow_no_phone
   *  @param queue_priority
   *  @param skip_dedup_check - If true, skip checking on existing supplier ID, phone numbers, etc.
   *  @return - the data from the api
  */
  char * PUTBusinessJson( char *json, char *country, char *timezone, char *master_entity_id, char *allow_no_address, char *allow_no_phone, char *queue_priority, char *skip_dedup_check) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"json=");
    p = curl_easy_escape(curl,json,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"timezone=");
    p = curl_easy_escape(curl,timezone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"master_entity_id=");
    p = curl_easy_escape(curl,master_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"allow_no_address=");
    p = curl_easy_escape(curl,allow_no_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"allow_no_phone=");
    p = curl_easy_escape(curl,allow_no_phone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"queue_priority=");
    p = curl_easy_escape(curl,queue_priority,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip_dedup_check=");
    p = curl_easy_escape(curl,skip_dedup_check,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/business/json",params);
  }


  /**
   * Create entity via JSON
   *
   *  @param entity_id - The entity to add rich data too
   *  @param json - The rich data to add to the entity
   *  @return - the data from the api
  */
  char * POSTBusinessJsonProcess( char *entity_id, char *json) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"json=");
    p = curl_easy_escape(curl,json,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/business/json/process",params);
  }


  /**
   * Delete a business tool with a specified tool_id
   *
   *  @param tool_id
   *  @return - the data from the api
  */
  char * DELETEBusiness_tool( char *tool_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tool_id=");
    p = curl_easy_escape(curl,tool_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/business_tool",params);
  }


  /**
   * Returns business tool that matches a given tool id
   *
   *  @param tool_id
   *  @return - the data from the api
  */
  char * GETBusiness_tool( char *tool_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tool_id=");
    p = curl_easy_escape(curl,tool_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/business_tool",params);
  }


  /**
   * Update/Add a Business Tool
   *
   *  @param tool_id
   *  @param country
   *  @param headline
   *  @param description
   *  @param link_url
   *  @param active
   *  @return - the data from the api
  */
  char * POSTBusiness_tool( char *tool_id, char *country, char *headline, char *description, char *link_url, char *active) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tool_id=");
    p = curl_easy_escape(curl,tool_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"headline=");
    p = curl_easy_escape(curl,headline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link_url=");
    p = curl_easy_escape(curl,link_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/business_tool",params);
  }


  /**
   * Returns active business tools for a specific masheryid in a given country
   *
   *  @param country
   *  @param activeonly
   *  @return - the data from the api
  */
  char * GETBusiness_toolBy_masheryid( char *country, char *activeonly) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"activeonly=");
    p = curl_easy_escape(curl,activeonly,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/business_tool/by_masheryid",params);
  }


  /**
   * Assigns a Call To Action to a Business Tool
   *
   *  @param tool_id
   *  @param enablecta
   *  @param cta_id
   *  @param slug
   *  @param nomodal
   *  @param type
   *  @param headline
   *  @param textshort
   *  @param link
   *  @param linklabel
   *  @param textlong
   *  @param textoutro
   *  @param bullets
   *  @param masheryids
   *  @param imgurl
   *  @param custombranding
   *  @param customcol
   *  @param custombkg
   *  @param customctacol
   *  @param customctabkg
   *  @param custominfocol
   *  @param custominfobkg
   *  @return - the data from the api
  */
  char * POSTBusiness_toolCta( char *tool_id, char *enablecta, char *cta_id, char *slug, char *nomodal, char *type, char *headline, char *textshort, char *link, char *linklabel, char *textlong, char *textoutro, char *bullets, char *masheryids, char *imgurl, char *custombranding, char *customcol, char *custombkg, char *customctacol, char *customctabkg, char *custominfocol, char *custominfobkg) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tool_id=");
    p = curl_easy_escape(curl,tool_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"enablecta=");
    p = curl_easy_escape(curl,enablecta,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"cta_id=");
    p = curl_easy_escape(curl,cta_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"slug=");
    p = curl_easy_escape(curl,slug,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"nomodal=");
    p = curl_easy_escape(curl,nomodal,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"headline=");
    p = curl_easy_escape(curl,headline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"textshort=");
    p = curl_easy_escape(curl,textshort,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link=");
    p = curl_easy_escape(curl,link,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"linklabel=");
    p = curl_easy_escape(curl,linklabel,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"textlong=");
    p = curl_easy_escape(curl,textlong,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"textoutro=");
    p = curl_easy_escape(curl,textoutro,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bullets=");
    p = curl_easy_escape(curl,bullets,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"masheryids=");
    p = curl_easy_escape(curl,masheryids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"imgurl=");
    p = curl_easy_escape(curl,imgurl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"custombranding=");
    p = curl_easy_escape(curl,custombranding,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"customcol=");
    p = curl_easy_escape(curl,customcol,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"custombkg=");
    p = curl_easy_escape(curl,custombkg,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"customctacol=");
    p = curl_easy_escape(curl,customctacol,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"customctabkg=");
    p = curl_easy_escape(curl,customctabkg,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"custominfocol=");
    p = curl_easy_escape(curl,custominfocol,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"custominfobkg=");
    p = curl_easy_escape(curl,custominfobkg,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/business_tool/cta",params);
  }


  /**
   * Assigns a Business Tool image
   *
   *  @param tool_id
   *  @param assignimage
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTBusiness_toolImage( char *tool_id, char *assignimage, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tool_id=");
    p = curl_easy_escape(curl,tool_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"assignimage=");
    p = curl_easy_escape(curl,assignimage,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/business_tool/image",params);
  }


  /**
   * Assigns a Business Tool image
   *
   *  @param tool_id
   *  @param image_url
   *  @return - the data from the api
  */
  char * POSTBusiness_toolImageBy_url( char *tool_id, char *image_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tool_id=");
    p = curl_easy_escape(curl,tool_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"image_url=");
    p = curl_easy_escape(curl,image_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/business_tool/image/by_url",params);
  }


  /**
   * With a known cache key get the data from cache
   *
   *  @param cache_key
   *  @param use_compression
   *  @return - the data from the api
  */
  char * GETCache( char *cache_key, char *use_compression) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"cache_key=");
    p = curl_easy_escape(curl,cache_key,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"use_compression=");
    p = curl_easy_escape(curl,use_compression,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/cache",params);
  }


  /**
   * Add some data to the cache with a given expiry
   *
   *  @param cache_key
   *  @param expiry - The cache expiry in seconds
   *  @param data - The data to cache
   *  @param use_compression
   *  @return - the data from the api
  */
  char * POSTCache( char *cache_key, char *expiry, char *data, char *use_compression) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"cache_key=");
    p = curl_easy_escape(curl,cache_key,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry=");
    p = curl_easy_escape(curl,expiry,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"use_compression=");
    p = curl_easy_escape(curl,use_compression,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/cache",params);
  }


  /**
   * Returns the supplied wolf category object by fetching the supplied category_id from our categories object.
   *
   *  @param category_id
   *  @return - the data from the api
  */
  char * GETCategory( char *category_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/category",params);
  }


  /**
   * With a known category id, an category object can be added.
   *
   *  @param category_id
   *  @param language
   *  @param name
   *  @return - the data from the api
  */
  char * PUTCategory( char *category_id, char *language, char *name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/category",params);
  }


  /**
   * Returns all Central Index categories and associated data
   *
   *  @param partner
   *  @return - the data from the api
  */
  char * GETCategoryAll( char *partner) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"partner=");
    p = curl_easy_escape(curl,partner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/category/all",params);
  }


  /**
   * With a known category id, a mapping object can be added.
   *
   *  @param category_id
   *  @param type
   *  @param id
   *  @param name
   *  @return - the data from the api
  */
  char * POSTCategoryMappings( char *category_id, char *type, char *id, char *name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/category/mappings",params);
  }


  /**
   * With a known category id, a mapping object can be deleted.
   *
   *  @param category_id
   *  @param category_type
   *  @param mapped_id
   *  @return - the data from the api
  */
  char * DELETECategoryMappings( char *category_id, char *category_type, char *mapped_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_type=");
    p = curl_easy_escape(curl,category_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapped_id=");
    p = curl_easy_escape(curl,mapped_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/category/mappings",params);
  }


  /**
   * Allows a category object to merged with another
   *
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * POSTCategoryMerge( char *from, char *to) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from=");
    p = curl_easy_escape(curl,from,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to=");
    p = curl_easy_escape(curl,to,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/category/merge",params);
  }


  /**
   * With a known category id, an synonym object can be added.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * POSTCategorySynonym( char *category_id, char *synonym, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"synonym=");
    p = curl_easy_escape(curl,synonym,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/category/synonym",params);
  }


  /**
   * With a known category id, a synonyms object can be removed.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * DELETECategorySynonym( char *category_id, char *synonym, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"synonym=");
    p = curl_easy_escape(curl,synonym,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/category/synonym",params);
  }


  /**
   * Get the contract from the ID supplied
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * GETContract( char *contract_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract",params);
  }


  /**
   * Get the active contracts from the ID supplied
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETContractBy_entity_id( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract/by_entity_id",params);
  }


  /**
   * Get a contract from the payment provider id supplied
   *
   *  @param payment_provider
   *  @param payment_provider_id
   *  @return - the data from the api
  */
  char * GETContractBy_payment_provider_id( char *payment_provider, char *payment_provider_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"payment_provider=");
    p = curl_easy_escape(curl,payment_provider,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_provider_id=");
    p = curl_easy_escape(curl,payment_provider_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract/by_payment_provider_id",params);
  }


  /**
   * Get the active contracts from the ID supplied
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETContractBy_user_id( char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract/by_user_id",params);
  }


  /**
   * Cancels an existing contract for a given id
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * POSTContractCancel( char *contract_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/cancel",params);
  }


  /**
   * Creates a new contract for a given entity
   *
   *  @param entity_id
   *  @param user_id
   *  @param payment_provider
   *  @param basket
   *  @param taxrate
   *  @param billing_period
   *  @param source
   *  @param channel
   *  @param campaign
   *  @param referrer_domain
   *  @param referrer_name
   *  @param flatpack_id
   *  @return - the data from the api
  */
  char * POSTContractCreate( char *entity_id, char *user_id, char *payment_provider, char *basket, char *taxrate, char *billing_period, char *source, char *channel, char *campaign, char *referrer_domain, char *referrer_name, char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_provider=");
    p = curl_easy_escape(curl,payment_provider,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"basket=");
    p = curl_easy_escape(curl,basket,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"taxrate=");
    p = curl_easy_escape(curl,taxrate,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"billing_period=");
    p = curl_easy_escape(curl,billing_period,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source=");
    p = curl_easy_escape(curl,source,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"channel=");
    p = curl_easy_escape(curl,channel,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"campaign=");
    p = curl_easy_escape(curl,campaign,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_domain=");
    p = curl_easy_escape(curl,referrer_domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_name=");
    p = curl_easy_escape(curl,referrer_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/create",params);
  }


  /**
   * Activate a contract that is free
   *
   *  @param contract_id
   *  @param user_name
   *  @param user_surname
   *  @param user_email_address
   *  @return - the data from the api
  */
  char * POSTContractFreeactivate( char *contract_id, char *user_name, char *user_surname, char *user_email_address) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_name=");
    p = curl_easy_escape(curl,user_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_surname=");
    p = curl_easy_escape(curl,user_surname,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_email_address=");
    p = curl_easy_escape(curl,user_email_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/freeactivate",params);
  }


  /**
   * When we failed to receive money add the dates etc to the contract
   *
   *  @param contract_id
   *  @param failure_reason
   *  @param payment_date
   *  @param amount
   *  @param currency
   *  @param response
   *  @return - the data from the api
  */
  char * POSTContractPaymentFailure( char *contract_id, char *failure_reason, char *payment_date, char *amount, char *currency, char *response) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"failure_reason=");
    p = curl_easy_escape(curl,failure_reason,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_date=");
    p = curl_easy_escape(curl,payment_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"amount=");
    p = curl_easy_escape(curl,amount,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency=");
    p = curl_easy_escape(curl,currency,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"response=");
    p = curl_easy_escape(curl,response,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/payment/failure",params);
  }


  /**
   * Adds payment details to a given contract_id
   *
   *  @param contract_id
   *  @param payment_provider_id
   *  @param payment_provider_profile
   *  @param user_name
   *  @param user_surname
   *  @param user_billing_address
   *  @param user_email_address
   *  @return - the data from the api
  */
  char * POSTContractPaymentSetup( char *contract_id, char *payment_provider_id, char *payment_provider_profile, char *user_name, char *user_surname, char *user_billing_address, char *user_email_address) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_provider_id=");
    p = curl_easy_escape(curl,payment_provider_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_provider_profile=");
    p = curl_easy_escape(curl,payment_provider_profile,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_name=");
    p = curl_easy_escape(curl,user_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_surname=");
    p = curl_easy_escape(curl,user_surname,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_billing_address=");
    p = curl_easy_escape(curl,user_billing_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_email_address=");
    p = curl_easy_escape(curl,user_email_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/payment/setup",params);
  }


  /**
   * When we receive money add the dates etc to the contract
   *
   *  @param contract_id
   *  @param payment_date
   *  @param amount
   *  @param currency
   *  @param response
   *  @return - the data from the api
  */
  char * POSTContractPaymentSuccess( char *contract_id, char *payment_date, char *amount, char *currency, char *response) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_date=");
    p = curl_easy_escape(curl,payment_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"amount=");
    p = curl_easy_escape(curl,amount,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency=");
    p = curl_easy_escape(curl,currency,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"response=");
    p = curl_easy_escape(curl,response,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/payment/success",params);
  }


  /**
   * Go through all the products in a contract and provision them
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * POSTContractProvision( char *contract_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/provision",params);
  }


  /**
   * Ensures contract has been cancelled for a given id, expected to be called from stripe on deletion of subscription
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * POSTContractSubscriptionended( char *contract_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract/subscriptionended",params);
  }


  /**
   * Get the contract log from the ID supplied
   *
   *  @param contract_log_id
   *  @return - the data from the api
  */
  char * GETContract_log( char *contract_log_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_log_id=");
    p = curl_easy_escape(curl,contract_log_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract_log",params);
  }


  /**
   * Creates a new contract log for a given contract
   *
   *  @param contract_id
   *  @param date
   *  @param payment_provider
   *  @param response
   *  @param success
   *  @param amount
   *  @param currency
   *  @return - the data from the api
  */
  char * POSTContract_log( char *contract_id, char *date, char *payment_provider, char *response, char *success, char *amount, char *currency) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"date=");
    p = curl_easy_escape(curl,date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_provider=");
    p = curl_easy_escape(curl,payment_provider,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"response=");
    p = curl_easy_escape(curl,response,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"success=");
    p = curl_easy_escape(curl,success,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"amount=");
    p = curl_easy_escape(curl,amount,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency=");
    p = curl_easy_escape(curl,currency,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/contract_log",params);
  }


  /**
   * Get the contract logs from the ID supplied
   *
   *  @param contract_id
   *  @param page
   *  @param per_page
   *  @return - the data from the api
  */
  char * GETContract_logBy_contract_id( char *contract_id, char *page, char *per_page) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract_log/by_contract_id",params);
  }


  /**
   * Get the contract logs from the payment_provider supplied
   *
   *  @param payment_provider
   *  @param page
   *  @param per_page
   *  @return - the data from the api
  */
  char * GETContract_logBy_payment_provider( char *payment_provider, char *page, char *per_page) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"payment_provider=");
    p = curl_easy_escape(curl,payment_provider,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/contract_log/by_payment_provider",params);
  }


  /**
   * Update/Add a country
   *
   *  @param country_id
   *  @param name
   *  @param synonyms
   *  @param continentName
   *  @param continent
   *  @param geonameId
   *  @param dbpediaURL
   *  @param freebaseURL
   *  @param population
   *  @param currencyCode
   *  @param languages
   *  @param areaInSqKm
   *  @param capital
   *  @param east
   *  @param west
   *  @param north
   *  @param south
   *  @param claimProductId
   *  @param claimMethods
   *  @param twilio_sms
   *  @param twilio_phone
   *  @param twilio_voice
   *  @param currency_symbol - the symbol of this country's currency
   *  @param currency_symbol_html - the html version of the symbol of this country's currency
   *  @param postcodeLookupActive - Whether the lookup is activated for this country
   *  @param addressFields - Whether fields are activated for this country
   *  @param addressMatching - The configurable matching algorithm
   *  @param dateFormat - The format of the date for this country
   *  @param iso_3166_alpha_3
   *  @param iso_3166_numeric
   *  @return - the data from the api
  */
  char * POSTCountry( char *country_id, char *name, char *synonyms, char *continentName, char *continent, char *geonameId, char *dbpediaURL, char *freebaseURL, char *population, char *currencyCode, char *languages, char *areaInSqKm, char *capital, char *east, char *west, char *north, char *south, char *claimProductId, char *claimMethods, char *twilio_sms, char *twilio_phone, char *twilio_voice, char *currency_symbol, char *currency_symbol_html, char *postcodeLookupActive, char *addressFields, char *addressMatching, char *dateFormat, char *iso_3166_alpha_3, char *iso_3166_numeric) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country_id=");
    p = curl_easy_escape(curl,country_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"synonyms=");
    p = curl_easy_escape(curl,synonyms,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"continentName=");
    p = curl_easy_escape(curl,continentName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"continent=");
    p = curl_easy_escape(curl,continent,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"geonameId=");
    p = curl_easy_escape(curl,geonameId,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"dbpediaURL=");
    p = curl_easy_escape(curl,dbpediaURL,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"freebaseURL=");
    p = curl_easy_escape(curl,freebaseURL,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"population=");
    p = curl_easy_escape(curl,population,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currencyCode=");
    p = curl_easy_escape(curl,currencyCode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"languages=");
    p = curl_easy_escape(curl,languages,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"areaInSqKm=");
    p = curl_easy_escape(curl,areaInSqKm,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"capital=");
    p = curl_easy_escape(curl,capital,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"east=");
    p = curl_easy_escape(curl,east,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"west=");
    p = curl_easy_escape(curl,west,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"north=");
    p = curl_easy_escape(curl,north,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"south=");
    p = curl_easy_escape(curl,south,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimProductId=");
    p = curl_easy_escape(curl,claimProductId,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimMethods=");
    p = curl_easy_escape(curl,claimMethods,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"twilio_sms=");
    p = curl_easy_escape(curl,twilio_sms,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"twilio_phone=");
    p = curl_easy_escape(curl,twilio_phone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"twilio_voice=");
    p = curl_easy_escape(curl,twilio_voice,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency_symbol=");
    p = curl_easy_escape(curl,currency_symbol,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency_symbol_html=");
    p = curl_easy_escape(curl,currency_symbol_html,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcodeLookupActive=");
    p = curl_easy_escape(curl,postcodeLookupActive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"addressFields=");
    p = curl_easy_escape(curl,addressFields,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"addressMatching=");
    p = curl_easy_escape(curl,addressMatching,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"dateFormat=");
    p = curl_easy_escape(curl,dateFormat,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"iso_3166_alpha_3=");
    p = curl_easy_escape(curl,iso_3166_alpha_3,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"iso_3166_numeric=");
    p = curl_easy_escape(curl,iso_3166_numeric,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/country",params);
  }


  /**
   * Fetching a country
   *
   *  @param country_id
   *  @return - the data from the api
  */
  char * GETCountry( char *country_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country_id=");
    p = curl_easy_escape(curl,country_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/country",params);
  }


  /**
   * An API call to fetch a crash report by its ID
   *
   *  @param crash_report_id - The crash report to pull
   *  @return - the data from the api
  */
  char * GETCrash_report( char *crash_report_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"crash_report_id=");
    p = curl_easy_escape(curl,crash_report_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/crash_report",params);
  }


  /**
   * Send an email via amazon
   *
   *  @param to_email_address - The email address to send the email too
   *  @param reply_email_address - The email address to add in the reply to field
   *  @param source_account - The source account to send the email from
   *  @param subject - The subject for the email
   *  @param body - The body for the email
   *  @param html_body - If the body of the email is html
   *  @return - the data from the api
  */
  char * POSTEmail( char *to_email_address, char *reply_email_address, char *source_account, char *subject, char *body, char *html_body) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"to_email_address=");
    p = curl_easy_escape(curl,to_email_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reply_email_address=");
    p = curl_easy_escape(curl,reply_email_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source_account=");
    p = curl_easy_escape(curl,source_account,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"subject=");
    p = curl_easy_escape(curl,subject,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"html_body=");
    p = curl_easy_escape(curl,html_body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/email",params);
  }


  /**
   * This entity isn't really supported anymore. You probably want PUT /business. Only to be used for testing.
   *
   *  @param type
   *  @param scope
   *  @param country
   *  @param timezone
   *  @param trust
   *  @param our_data
   *  @return - the data from the api
  */
  char * PUTEntity( char *type, char *scope, char *country, char *timezone, char *trust, char *our_data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"scope=");
    p = curl_easy_escape(curl,scope,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"timezone=");
    p = curl_easy_escape(curl,timezone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"trust=");
    p = curl_easy_escape(curl,trust,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"our_data=");
    p = curl_easy_escape(curl,our_data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/entity",params);
  }


  /**
   * Allows a whole entity to be pulled from the datastore by its unique id
   *
   *  @param entity_id - The unique entity ID e.g. 379236608286720
   *  @param domain
   *  @param path
   *  @param data_filter
   *  @param filter_by_confidence
   *  @return - the data from the api
  */
  char * GETEntity( char *entity_id, char *domain, char *path, char *data_filter, char *filter_by_confidence) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data_filter=");
    p = curl_easy_escape(curl,data_filter,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filter_by_confidence=");
    p = curl_easy_escape(curl,filter_by_confidence,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity",params);
  }


  /**
   * With a known entity id, an add can be updated.
   *
   *  @param entity_id
   *  @param add_referrer_url
   *  @param add_referrer_name
   *  @return - the data from the api
  */
  char * POSTEntityAdd( char *entity_id, char *add_referrer_url, char *add_referrer_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"add_referrer_url=");
    p = curl_easy_escape(curl,add_referrer_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"add_referrer_name=");
    p = curl_easy_escape(curl,add_referrer_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/add",params);
  }


  /**
   * Allows an advertiser object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAdvertiser( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/advertiser",params);
  }


  /**
   * Expires an advertiser from and entity
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserCancel( char *entity_id, char *publisher_id, char *reseller_ref, char *reseller_agent_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/cancel",params);
  }


  /**
   * With a known entity id, a advertiser is added
   *
   *  @param entity_id
   *  @param tags
   *  @param locations
   *  @param loc_tags
   *  @param region_tags
   *  @param max_tags
   *  @param max_locations
   *  @param expiry_date
   *  @param is_national
   *  @param is_regional
   *  @param language
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserCreate( char *entity_id, char *tags, char *locations, char *loc_tags, char *region_tags, char *max_tags, char *max_locations, char *expiry_date, char *is_national, char *is_regional, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tags=");
    p = curl_easy_escape(curl,tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"locations=");
    p = curl_easy_escape(curl,locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"loc_tags=");
    p = curl_easy_escape(curl,loc_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"region_tags=");
    p = curl_easy_escape(curl,region_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_tags=");
    p = curl_easy_escape(curl,max_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_locations=");
    p = curl_easy_escape(curl,max_locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_national=");
    p = curl_easy_escape(curl,is_national,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_regional=");
    p = curl_easy_escape(curl,is_regional,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/create",params);
  }


  /**
   * Adds/removes locations
   *
   *  @param entity_id
   *  @param gen_id
   *  @param locations_to_add
   *  @param locations_to_remove
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserLocation( char *entity_id, char *gen_id, char *locations_to_add, char *locations_to_remove) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"locations_to_add=");
    p = curl_easy_escape(curl,locations_to_add,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"locations_to_remove=");
    p = curl_easy_escape(curl,locations_to_remove,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/location",params);
  }


  /**
   * With a known entity id, a premium advertiser is cancelled
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param type
   *  @param category_id - The category of the advertiser
   *  @param location_id - The location of the advertiser
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserPremiumCancel( char *entity_id, char *publisher_id, char *type, char *category_id, char *location_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/premium/cancel",params);
  }


  /**
   * With a known entity id, a premium advertiser is added
   *
   *  @param entity_id
   *  @param type
   *  @param category_id - The category of the advertiser
   *  @param location_id - The location of the advertiser
   *  @param expiry_date
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserPremiumCreate( char *entity_id, char *type, char *category_id, char *location_id, char *expiry_date, char *reseller_ref, char *reseller_agent_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/premium/create",params);
  }


  /**
   * Renews an existing premium advertiser in an entity
   *
   *  @param entity_id
   *  @param type
   *  @param category_id - The category of the advertiser
   *  @param location_id - The location of the advertiser
   *  @param expiry_date
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserPremiumRenew( char *entity_id, char *type, char *category_id, char *location_id, char *expiry_date, char *reseller_ref, char *reseller_agent_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/premium/renew",params);
  }


  /**
   * Renews an advertiser from an entity
   *
   *  @param entity_id
   *  @param expiry_date
   *  @param publisher_id
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserRenew( char *entity_id, char *expiry_date, char *publisher_id, char *reseller_ref, char *reseller_agent_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/renew",params);
  }


  /**
   * Allows the removal or insertion of tags into an advertiser object
   *
   *  @param gen_id - The gen_id of this advertiser
   *  @param entity_id - The entity_id of the advertiser
   *  @param language - The tag language to alter
   *  @param tags_to_add - The tags to add
   *  @param tags_to_remove - The tags to remove
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserTag( char *gen_id, char *entity_id, char *language, char *tags_to_add, char *tags_to_remove) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tags_to_add=");
    p = curl_easy_escape(curl,tags_to_add,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tags_to_remove=");
    p = curl_easy_escape(curl,tags_to_remove,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/tag",params);
  }


  /**
   * With a known entity id, an advertiser is updated
   *
   *  @param entity_id
   *  @param tags
   *  @param locations
   *  @param loc_tags
   *  @param is_regional
   *  @param region_tags
   *  @param extra_tags
   *  @param extra_locations
   *  @param is_national
   *  @param language
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserUpsell( char *entity_id, char *tags, char *locations, char *loc_tags, char *is_regional, char *region_tags, char *extra_tags, char *extra_locations, char *is_national, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tags=");
    p = curl_easy_escape(curl,tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"locations=");
    p = curl_easy_escape(curl,locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"loc_tags=");
    p = curl_easy_escape(curl,loc_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_regional=");
    p = curl_easy_escape(curl,is_regional,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"region_tags=");
    p = curl_easy_escape(curl,region_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"extra_tags=");
    p = curl_easy_escape(curl,extra_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"extra_locations=");
    p = curl_easy_escape(curl,extra_locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_national=");
    p = curl_easy_escape(curl,is_national,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/advertiser/upsell",params);
  }


  /**
   * Search for matching entities that are advertisers and return a random selection upto the limit requested
   *
   *  @param tag - The word or words the advertiser is to appear for in searches
   *  @param where - The location to get results for. E.g. Dublin
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param is_national
   *  @param limit - The number of advertisers that are to be returned
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param benchmark
   *  @return - the data from the api
  */
  char * GETEntityAdvertisers( char *tag, char *where, char *orderonline, char *delivers, char *isClaimed, char *is_national, char *limit, char *country, char *language, char *benchmark) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"tag=");
    p = curl_easy_escape(curl,tag,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_national=");
    p = curl_easy_escape(curl,is_national,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"benchmark=");
    p = curl_easy_escape(curl,benchmark,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/advertisers",params);
  }


  /**
   * Search for matching entities in a specified location that are advertisers and return a random selection upto the limit requested
   *
   *  @param location - The location to get results for. E.g. Dublin
   *  @param is_national
   *  @param limit - The number of advertisers that are to be returned
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * GETEntityAdvertisersBy_location( char *location, char *is_national, char *limit, char *country, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"location=");
    p = curl_easy_escape(curl,location,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_national=");
    p = curl_easy_escape(curl,is_national,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/advertisers/by_location",params);
  }


  /**
   * Check if an entity has an advert from a specified publisher
   *
   *  @param entity_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * GETEntityAdvertisersInventorycheck( char *entity_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/advertisers/inventorycheck",params);
  }


  /**
   * Get advertisers premium
   *
   *  @param what
   *  @param where
   *  @param type
   *  @param country
   *  @param language
   *  @return - the data from the api
  */
  char * GETEntityAdvertisersPremium( char *what, char *where, char *type, char *country, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"what=");
    p = curl_easy_escape(curl,what,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/advertisers/premium",params);
  }


  /**
   * Deleteing an affiliate adblock from a known entity
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAffiliate_adblock( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/affiliate_adblock",params);
  }


  /**
   * Adding an affiliate adblock to a known entity
   *
   *  @param entity_id
   *  @param adblock - Number of results returned per page
   *  @return - the data from the api
  */
  char * POSTEntityAffiliate_adblock( char *entity_id, char *adblock) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adblock=");
    p = curl_easy_escape(curl,adblock,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/affiliate_adblock",params);
  }


  /**
   * With a known entity id, an affiliate link object can be added.
   *
   *  @param entity_id
   *  @param affiliate_name
   *  @param affiliate_link
   *  @param affiliate_message
   *  @param affiliate_logo
   *  @param affiliate_action
   *  @return - the data from the api
  */
  char * POSTEntityAffiliate_link( char *entity_id, char *affiliate_name, char *affiliate_link, char *affiliate_message, char *affiliate_logo, char *affiliate_action) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"affiliate_name=");
    p = curl_easy_escape(curl,affiliate_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"affiliate_link=");
    p = curl_easy_escape(curl,affiliate_link,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"affiliate_message=");
    p = curl_easy_escape(curl,affiliate_message,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"affiliate_logo=");
    p = curl_easy_escape(curl,affiliate_logo,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"affiliate_action=");
    p = curl_easy_escape(curl,affiliate_action,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/affiliate_link",params);
  }


  /**
   * Allows an affiliate link object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAffiliate_link( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/affiliate_link",params);
  }


  /**
   * Add/edit an annoucement object to an existing entity.
   *
   *  @param entity_id
   *  @param announcement_id
   *  @param headline
   *  @param body
   *  @param link_label
   *  @param link
   *  @param terms_link
   *  @param publish_date
   *  @param expiry_date
   *  @param media_type
   *  @param image_url
   *  @param video_url
   *  @param type - Type of announcement, which affects how it is displayed.
   *  @return - the data from the api
  */
  char * POSTEntityAnnouncement( char *entity_id, char *announcement_id, char *headline, char *body, char *link_label, char *link, char *terms_link, char *publish_date, char *expiry_date, char *media_type, char *image_url, char *video_url, char *type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"announcement_id=");
    p = curl_easy_escape(curl,announcement_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"headline=");
    p = curl_easy_escape(curl,headline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link_label=");
    p = curl_easy_escape(curl,link_label,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link=");
    p = curl_easy_escape(curl,link,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"terms_link=");
    p = curl_easy_escape(curl,terms_link,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publish_date=");
    p = curl_easy_escape(curl,publish_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"media_type=");
    p = curl_easy_escape(curl,media_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"image_url=");
    p = curl_easy_escape(curl,image_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"video_url=");
    p = curl_easy_escape(curl,video_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/announcement",params);
  }


  /**
   * Fetch an announcement object from an existing entity.
   *
   *  @param entity_id
   *  @param announcement_id
   *  @return - the data from the api
  */
  char * GETEntityAnnouncement( char *entity_id, char *announcement_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"announcement_id=");
    p = curl_easy_escape(curl,announcement_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/announcement",params);
  }


  /**
   * Remove an announcement object to an existing entity.
   *
   *  @param entity_id
   *  @param announcement_id
   *  @return - the data from the api
  */
  char * DELETEEntityAnnouncement( char *entity_id, char *announcement_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"announcement_id=");
    p = curl_easy_escape(curl,announcement_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/announcement",params);
  }


  /**
   * Will create a new association_membership or update an existing one
   *
   *  @param entity_id
   *  @param association_id
   *  @param association_member_url
   *  @param association_member_id
   *  @return - the data from the api
  */
  char * POSTEntityAssociation_membership( char *entity_id, char *association_id, char *association_member_url, char *association_member_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"association_id=");
    p = curl_easy_escape(curl,association_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"association_member_url=");
    p = curl_easy_escape(curl,association_member_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"association_member_id=");
    p = curl_easy_escape(curl,association_member_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/association_membership",params);
  }


  /**
   * Allows a association_membership object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAssociation_membership( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/association_membership",params);
  }


  /**
   * With a known entity id, an background object can be added. There can however only be one background object.
   *
   *  @param entity_id
   *  @param number_of_employees
   *  @param turnover
   *  @param net_profit
   *  @param vat_number
   *  @param duns_number
   *  @param registered_company_number
   *  @return - the data from the api
  */
  char * POSTEntityBackground( char *entity_id, char *number_of_employees, char *turnover, char *net_profit, char *vat_number, char *duns_number, char *registered_company_number) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"number_of_employees=");
    p = curl_easy_escape(curl,number_of_employees,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"turnover=");
    p = curl_easy_escape(curl,turnover,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"net_profit=");
    p = curl_easy_escape(curl,net_profit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"vat_number=");
    p = curl_easy_escape(curl,vat_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"duns_number=");
    p = curl_easy_escape(curl,duns_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"registered_company_number=");
    p = curl_easy_escape(curl,registered_company_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/background",params);
  }


  /**
   * With a known entity id, a brand object can be added.
   *
   *  @param entity_id
   *  @param value
   *  @return - the data from the api
  */
  char * POSTEntityBrand( char *entity_id, char *value) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"value=");
    p = curl_easy_escape(curl,value,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/brand",params);
  }


  /**
   * With a known entity id, a brand object can be deleted.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityBrand( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/brand",params);
  }


  /**
   * Uploads a CSV file of known format and bulk inserts into DB
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTEntityBulkCsv( char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/bulk/csv",params);
  }


  /**
   * Shows the current status of a bulk upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * GETEntityBulkCsvStatus( char *upload_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"upload_id=");
    p = curl_easy_escape(curl,upload_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/bulk/csv/status",params);
  }


  /**
   * Uploads a JSON file of known format and bulk inserts into DB
   *
   *  @param data
   *  @param new_entities
   *  @return - the data from the api
  */
  char * POSTEntityBulkJson( char *data, char *new_entities) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"new_entities=");
    p = curl_easy_escape(curl,new_entities,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/bulk/json",params);
  }


  /**
   * Shows the current status of a bulk JSON upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * GETEntityBulkJsonStatus( char *upload_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"upload_id=");
    p = curl_easy_escape(curl,upload_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/bulk/json/status",params);
  }


  /**
   * Fetches the document that matches the given data_source_type and external_id.
   *
   *  @param data_source_type - The data source type of the entity
   *  @param external_id - The external ID of the entity
   *  @return - the data from the api
  */
  char * GETEntityBy_external_id( char *data_source_type, char *external_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"data_source_type=");
    p = curl_easy_escape(curl,data_source_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"external_id=");
    p = curl_easy_escape(curl,external_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/by_external_id",params);
  }


  /**
   * Get all entities within a specified group
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * GETEntityBy_groupid( char *group_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/by_groupid",params);
  }


  /**
   * Fetches the document that matches the given legacy_url
   *
   *  @param legacy_url - The URL of the entity in the directory it was imported from.
   *  @return - the data from the api
  */
  char * GETEntityBy_legacy_url( char *legacy_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"legacy_url=");
    p = curl_easy_escape(curl,legacy_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/by_legacy_url",params);
  }


  /**
   * uncontributes a given entities supplier content and makes the entity inactive if the entity is un-usable
   *
   *  @param entity_id - The entity to pull
   *  @param supplier_masheryid - The suppliers masheryid to match
   *  @param supplier_id - The supplier id to match
   *  @param supplier_user_id - The user id to match
   *  @return - the data from the api
  */
  char * DELETEEntityBy_supplier( char *entity_id, char *supplier_masheryid, char *supplier_id, char *supplier_user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_masheryid=");
    p = curl_easy_escape(curl,supplier_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_id=");
    p = curl_easy_escape(curl,supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_user_id=");
    p = curl_easy_escape(curl,supplier_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/by_supplier",params);
  }


  /**
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID, or a list of supplier IDs separated by comma
   *  @return - the data from the api
  */
  char * GETEntityBy_supplier_id( char *supplier_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"supplier_id=");
    p = curl_easy_escape(curl,supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/by_supplier_id",params);
  }


  /**
   * Get all entities added or claimed by a specific user
   *
   *  @param user_id - The unique user ID of the user with claimed entities e.g. 379236608286720
   *  @param filter
   *  @param skip
   *  @param limit
   *  @return - the data from the api
  */
  char * GETEntityBy_user_id( char *user_id, char *filter, char *skip, char *limit) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filter=");
    p = curl_easy_escape(curl,filter,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip=");
    p = curl_easy_escape(curl,skip,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/by_user_id",params);
  }


  /**
   * With a known entity id, an category object can be added.
   *
   *  @param entity_id
   *  @param category_id
   *  @param category_type
   *  @return - the data from the api
  */
  char * POSTEntityCategory( char *entity_id, char *category_id, char *category_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_type=");
    p = curl_easy_escape(curl,category_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/category",params);
  }


  /**
   * Allows a category object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityCategory( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/category",params);
  }


  /**
   * Fetches the changelog documents that match the given entity_id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETEntityChangelog( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/changelog",params);
  }


  /**
   * Unlike cancel, this operation remove the claim data from the entity
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityClaim( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/claim",params);
  }


  /**
   * Allow an entity to be claimed by a valid user
   *
   *  @param entity_id
   *  @param claimed_user_id
   *  @param claimed_reseller_id
   *  @param expiry_date
   *  @param claimed_date
   *  @param verified_status - If set to a value, this field will promote the claim to pro mode (expiry aligned with claim expiry)
   *  @param claim_method
   *  @param phone_number
   *  @param referrer_url
   *  @param referrer_name
   *  @param reseller_ref
   *  @param reseller_description
   *  @return - the data from the api
  */
  char * POSTEntityClaim( char *entity_id, char *claimed_user_id, char *claimed_reseller_id, char *expiry_date, char *claimed_date, char *verified_status, char *claim_method, char *phone_number, char *referrer_url, char *referrer_name, char *reseller_ref, char *reseller_description) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimed_user_id=");
    p = curl_easy_escape(curl,claimed_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimed_reseller_id=");
    p = curl_easy_escape(curl,claimed_reseller_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimed_date=");
    p = curl_easy_escape(curl,claimed_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"verified_status=");
    p = curl_easy_escape(curl,verified_status,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claim_method=");
    p = curl_easy_escape(curl,claim_method,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"phone_number=");
    p = curl_easy_escape(curl,phone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_url=");
    p = curl_easy_escape(curl,referrer_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_name=");
    p = curl_easy_escape(curl,referrer_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_description=");
    p = curl_easy_escape(curl,reseller_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/claim",params);
  }


  /**
   * Cancel a claim that is on the entity
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTEntityClaimCancel( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/claim/cancel",params);
  }


  /**
   * Allow an entity to be claimed by a valid user
   *
   *  @param entity_id
   *  @param claimed_user_id
   *  @param reseller_ref
   *  @param reseller_description
   *  @param expiry_date
   *  @param renew_verify - Update the verified_status (where present) as well. Paid claims should do this -- free claims generally will not.
   *  @return - the data from the api
  */
  char * POSTEntityClaimRenew( char *entity_id, char *claimed_user_id, char *reseller_ref, char *reseller_description, char *expiry_date, char *renew_verify) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimed_user_id=");
    p = curl_easy_escape(curl,claimed_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_description=");
    p = curl_easy_escape(curl,reseller_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"renew_verify=");
    p = curl_easy_escape(curl,renew_verify,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/claim/renew",params);
  }


  /**
   * Allow an entity to be claimed by a valid reseller
   *
   *  @param entity_id
   *  @param reseller_ref
   *  @param reseller_description
   *  @return - the data from the api
  */
  char * POSTEntityClaimReseller( char *entity_id, char *reseller_ref, char *reseller_description) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_description=");
    p = curl_easy_escape(curl,reseller_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/claim/reseller",params);
  }


  /**
   * If an entity is currently claimed then set or remove the verified_entity block (Expiry will match the claim expiry)
   *
   *  @param entity_id
   *  @param verified_status - If set to a value, this field will promote the claim to pro mode. If blank, verified status will be wiped
   *  @return - the data from the api
  */
  char * POSTEntityClaimVerfied_status( char *entity_id, char *verified_status) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"verified_status=");
    p = curl_easy_escape(curl,verified_status,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/claim/verfied_status",params);
  }


  /**
   * Add/change delivers flag for an existing entity - to indicate whether business offers delivery
   *
   *  @param entity_id
   *  @param delivers
   *  @return - the data from the api
  */
  char * POSTEntityDelivers( char *entity_id, char *delivers) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/delivers",params);
  }


  /**
   * Allows a description object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityDescription( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/description",params);
  }


  /**
   * With a known entity id, a description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @param gen_id
   *  @return - the data from the api
  */
  char * POSTEntityDescription( char *entity_id, char *headline, char *body, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"headline=");
    p = curl_easy_escape(curl,headline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/description",params);
  }


  /**
   * With a known entity id, an document object can be added.
   *
   *  @param entity_id
   *  @param name
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTEntityDocument( char *entity_id, char *name, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/document",params);
  }


  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityDocument( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/document",params);
  }


  /**
   * Upload a document to an entity
   *
   *  @param entity_id
   *  @param document
   *  @return - the data from the api
  */
  char * POSTEntityDocumentBy_url( char *entity_id, char *document) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"document=");
    p = curl_easy_escape(curl,document,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/document/by_url",params);
  }


  /**
   * Allows a email object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityEmail( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/email",params);
  }


  /**
   * With a known entity id, an email address object can be added.
   *
   *  @param entity_id
   *  @param email_address
   *  @param email_description
   *  @return - the data from the api
  */
  char * POSTEntityEmail( char *entity_id, char *email_address, char *email_description) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email_address=");
    p = curl_easy_escape(curl,email_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email_description=");
    p = curl_easy_escape(curl,email_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/email",params);
  }


  /**
   * Fetch an emergency statement object from an existing entity.
   *
   *  @param entity_id
   *  @param emergencystatement_id
   *  @return - the data from the api
  */
  char * GETEntityEmergencystatement( char *entity_id, char *emergencystatement_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"emergencystatement_id=");
    p = curl_easy_escape(curl,emergencystatement_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/emergencystatement",params);
  }


  /**
   * Add or update an emergency statement object to an existing entity.
   *
   *  @param entity_id
   *  @param id
   *  @param headline
   *  @param body
   *  @param link_label
   *  @param link
   *  @param publish_date
   *  @return - the data from the api
  */
  char * POSTEntityEmergencystatement( char *entity_id, char *id, char *headline, char *body, char *link_label, char *link, char *publish_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"headline=");
    p = curl_easy_escape(curl,headline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link_label=");
    p = curl_easy_escape(curl,link_label,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link=");
    p = curl_easy_escape(curl,link,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publish_date=");
    p = curl_easy_escape(curl,publish_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/emergencystatement",params);
  }


  /**
   * Remove an emergencystatement object to an existing entity.
   *
   *  @param entity_id
   *  @param emergencystatement_id
   *  @return - the data from the api
  */
  char * DELETEEntityEmergencystatement( char *entity_id, char *emergencystatement_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"emergencystatement_id=");
    p = curl_easy_escape(curl,emergencystatement_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/emergencystatement",params);
  }


  /**
   * Fetch emergency statement objects from an existing entity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETEntityEmergencystatements( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/emergencystatements",params);
  }


  /**
   * With a known entity id, an employee object can be added.
   *
   *  @param entity_id
   *  @param title
   *  @param forename
   *  @param surname
   *  @param job_title
   *  @param description
   *  @param email
   *  @param phone_number
   *  @return - the data from the api
  */
  char * POSTEntityEmployee( char *entity_id, char *title, char *forename, char *surname, char *job_title, char *description, char *email, char *phone_number) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"title=");
    p = curl_easy_escape(curl,title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"forename=");
    p = curl_easy_escape(curl,forename,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"surname=");
    p = curl_easy_escape(curl,surname,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"job_title=");
    p = curl_easy_escape(curl,job_title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"phone_number=");
    p = curl_easy_escape(curl,phone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/employee",params);
  }


  /**
   * Allows an employee object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityEmployee( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/employee",params);
  }


  /**
   * With a known entity id, an FAQ question and answer can be added.
   *
   *  @param entity_id
   *  @param question
   *  @param answer
   *  @param gen_id
   *  @return - the data from the api
  */
  char * POSTEntityFaq( char *entity_id, char *question, char *answer, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"question=");
    p = curl_easy_escape(curl,question,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"answer=");
    p = curl_easy_escape(curl,answer,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/faq",params);
  }


  /**
   * With a known entity id, an FAQ question and answer can be removed.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityFaq( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/faq",params);
  }


  /**
   * Allows a fax object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityFax( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/fax",params);
  }


  /**
   * With a known entity id, an fax object can be added.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @return - the data from the api
  */
  char * POSTEntityFax( char *entity_id, char *number, char *description) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"number=");
    p = curl_easy_escape(curl,number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/fax",params);
  }


  /**
   * With a known entity id, a featured message can be added
   *
   *  @param entity_id
   *  @param featured_text
   *  @param featured_url
   *  @return - the data from the api
  */
  char * POSTEntityFeatured_message( char *entity_id, char *featured_text, char *featured_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"featured_text=");
    p = curl_easy_escape(curl,featured_text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"featured_url=");
    p = curl_easy_escape(curl,featured_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/featured_message",params);
  }


  /**
   * Allows a featured message object to be removed
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityFeatured_message( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/featured_message",params);
  }


  /**
   * With a known entity id, a geopoint can be updated.
   *
   *  @param entity_id
   *  @param longitude
   *  @param latitude
   *  @param accuracy
   *  @return - the data from the api
  */
  char * POSTEntityGeopoint( char *entity_id, char *longitude, char *latitude, char *accuracy) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"accuracy=");
    p = curl_easy_escape(curl,accuracy,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/geopoint",params);
  }


  /**
   * With a known entity id, a group  can be added to group members.
   *
   *  @param entity_id
   *  @param group_id
   *  @return - the data from the api
  */
  char * POSTEntityGroup( char *entity_id, char *group_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/group",params);
  }


  /**
   * Allows a group object to be removed from an entities group members
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityGroup( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/group",params);
  }


  /**
   * With a known entity id, a image object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param image_name
   *  @return - the data from the api
  */
  char * POSTEntityImage( char *entity_id, char *filedata, char *image_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"image_name=");
    p = curl_easy_escape(curl,image_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/image",params);
  }


  /**
   * Allows a image object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityImage( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/image",params);
  }


  /**
   * With a known entity id, a image can be retrieved from a url and added.
   *
   *  @param entity_id
   *  @param image_url
   *  @param image_name
   *  @return - the data from the api
  */
  char * POSTEntityImageBy_url( char *entity_id, char *image_url, char *image_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"image_url=");
    p = curl_easy_escape(curl,image_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"image_name=");
    p = curl_easy_escape(curl,image_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/image/by_url",params);
  }


  /**
   * With a known entity id, an invoice_address object can be updated.
   *
   *  @param entity_id
   *  @param building_number
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param province
   *  @param postcode
   *  @param address_type
   *  @return - the data from the api
  */
  char * POSTEntityInvoice_address( char *entity_id, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *address_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"building_number=");
    p = curl_easy_escape(curl,building_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address1=");
    p = curl_easy_escape(curl,address1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address2=");
    p = curl_easy_escape(curl,address2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address3=");
    p = curl_easy_escape(curl,address3,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"district=");
    p = curl_easy_escape(curl,district,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"town=");
    p = curl_easy_escape(curl,town,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"county=");
    p = curl_easy_escape(curl,county,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"province=");
    p = curl_easy_escape(curl,province,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcode=");
    p = curl_easy_escape(curl,postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address_type=");
    p = curl_easy_escape(curl,address_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/invoice_address",params);
  }


  /**
   * With a known entity id and a known invoice_address ID, we can delete a specific invoice_address object from an enitity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityInvoice_address( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/invoice_address",params);
  }


  /**
   * With a known entity id, a language object can be deleted.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityLanguage( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/language",params);
  }


  /**
   * With a known entity id, a language object can be added.
   *
   *  @param entity_id
   *  @param value
   *  @return - the data from the api
  */
  char * POSTEntityLanguage( char *entity_id, char *value) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"value=");
    p = curl_easy_escape(curl,value,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/language",params);
  }


  /**
   * Allows a list description object to be reduced in confidence
   *
   *  @param gen_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityList( char *gen_id, char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/list",params);
  }


  /**
   * With a known entity id, a list description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @return - the data from the api
  */
  char * POSTEntityList( char *entity_id, char *headline, char *body) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"headline=");
    p = curl_easy_escape(curl,headline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/list",params);
  }


  /**
   * Find all entities in a group
   *
   *  @param group_id - A valid group_id
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @return - the data from the api
  */
  char * GETEntityList_by_group_id( char *group_id, char *per_page, char *page) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/list_by_group_id",params);
  }


  /**
   * Adds/removes loc_tags
   *
   *  @param entity_id
   *  @param gen_id
   *  @param loc_tags_to_add
   *  @param loc_tags_to_remove
   *  @return - the data from the api
  */
  char * POSTEntityLoc_tag( char *entity_id, char *gen_id, char *loc_tags_to_add, char *loc_tags_to_remove) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"loc_tags_to_add=");
    p = curl_easy_escape(curl,loc_tags_to_add,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"loc_tags_to_remove=");
    p = curl_easy_escape(curl,loc_tags_to_remove,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/loc_tag",params);
  }


  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityLogo( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/logo",params);
  }


  /**
   * With a known entity id, a logo object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param logo_name
   *  @return - the data from the api
  */
  char * POSTEntityLogo( char *entity_id, char *filedata, char *logo_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"logo_name=");
    p = curl_easy_escape(curl,logo_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/logo",params);
  }


  /**
   * With a known entity id, a logo can be retrieved from a url and added.
   *
   *  @param entity_id
   *  @param logo_url
   *  @param logo_name
   *  @return - the data from the api
  */
  char * POSTEntityLogoBy_url( char *entity_id, char *logo_url, char *logo_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"logo_url=");
    p = curl_easy_escape(curl,logo_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"logo_name=");
    p = curl_easy_escape(curl,logo_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/logo/by_url",params);
  }


  /**
   * Merge two entities into one
   *
   *  @param from
   *  @param to
   *  @param override_trust - Do you want to override the trust of the 'from' entity
   *  @param uncontribute_masheryid - Do we want to uncontribute any data for a masheryid?
   *  @param uncontribute_userid - Do we want to uncontribute any data for a user_id?
   *  @param uncontribute_supplierid - Do we want to uncontribute any data for a supplier_id?
   *  @param delete_mode - The type of object contribution deletion
   *  @return - the data from the api
  */
  char * POSTEntityMerge( char *from, char *to, char *override_trust, char *uncontribute_masheryid, char *uncontribute_userid, char *uncontribute_supplierid, char *delete_mode) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from=");
    p = curl_easy_escape(curl,from,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to=");
    p = curl_easy_escape(curl,to,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"override_trust=");
    p = curl_easy_escape(curl,override_trust,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"uncontribute_masheryid=");
    p = curl_easy_escape(curl,uncontribute_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"uncontribute_userid=");
    p = curl_easy_escape(curl,uncontribute_userid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"uncontribute_supplierid=");
    p = curl_easy_escape(curl,uncontribute_supplierid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delete_mode=");
    p = curl_easy_escape(curl,delete_mode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/merge",params);
  }


  /**
   * Update entities that use an old category ID to a new one
   *
   *  @param from
   *  @param to
   *  @param limit
   *  @return - the data from the api
  */
  char * POSTEntityMigrate_category( char *from, char *to, char *limit) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from=");
    p = curl_easy_escape(curl,from,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to=");
    p = curl_easy_escape(curl,to,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/migrate_category",params);
  }


  /**
   * With a known entity id, a name can be updated.
   *
   *  @param entity_id
   *  @param name
   *  @param formal_name
   *  @param branch_name
   *  @return - the data from the api
  */
  char * POSTEntityName( char *entity_id, char *name, char *formal_name, char *branch_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"formal_name=");
    p = curl_easy_escape(curl,formal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"branch_name=");
    p = curl_easy_escape(curl,branch_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/name",params);
  }


  /**
   * With a known entity id, a opening times object can be added. Each day can be either 'closed' to indicate that the entity is closed that day, '24hour' to indicate that the entity is open all day or single/split time ranges can be supplied in 4-digit 24-hour format, such as '09001730' or '09001200,13001700' to indicate hours of opening.
   *
   *  @param entity_id - The id of the entity to edit
   *  @param statement - Statement describing reasons for special opening/closing times
   *  @param monday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param tuesday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param wednesday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param thursday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param friday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param saturday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param sunday - e.g. 'closed', '24hour' , '09001730' , '09001200,13001700'
   *  @param closed - a comma-separated list of dates that the entity is closed e.g. '2013-04-29,2013-05-02'
   *  @param closed_public_holidays - whether the entity is closed on public holidays
   *  @return - the data from the api
  */
  char * POSTEntityOpening_times( char *entity_id, char *statement, char *monday, char *tuesday, char *wednesday, char *thursday, char *friday, char *saturday, char *sunday, char *closed, char *closed_public_holidays) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"statement=");
    p = curl_easy_escape(curl,statement,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"monday=");
    p = curl_easy_escape(curl,monday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tuesday=");
    p = curl_easy_escape(curl,tuesday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"wednesday=");
    p = curl_easy_escape(curl,wednesday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"thursday=");
    p = curl_easy_escape(curl,thursday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"friday=");
    p = curl_easy_escape(curl,friday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"saturday=");
    p = curl_easy_escape(curl,saturday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"sunday=");
    p = curl_easy_escape(curl,sunday,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"closed=");
    p = curl_easy_escape(curl,closed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"closed_public_holidays=");
    p = curl_easy_escape(curl,closed_public_holidays,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/opening_times",params);
  }


  /**
   * With a known entity id, a opening times object can be removed.
   *
   *  @param entity_id - The id of the entity to edit
   *  @return - the data from the api
  */
  char * DELETEEntityOpening_times( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/opening_times",params);
  }


  /**
   * Add an order online to an existing entity - to indicate e-commerce capability.
   *
   *  @param entity_id
   *  @param orderonline
   *  @return - the data from the api
  */
  char * POSTEntityOrderonline( char *entity_id, char *orderonline) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/orderonline",params);
  }


  /**
   * Allows a payment_type object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityPayment_type( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/payment_type",params);
  }


  /**
   * With a known entity id, a payment_type object can be added.
   *
   *  @param entity_id - the id of the entity to add the payment type to
   *  @param payment_type - the payment type to add to the entity
   *  @return - the data from the api
  */
  char * POSTEntityPayment_type( char *entity_id, char *payment_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"payment_type=");
    p = curl_easy_escape(curl,payment_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/payment_type",params);
  }


  /**
   * Allows a new phone object to be added to a specified entity. A new object id will be calculated and returned to you if successful.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @param trackable
   *  @return - the data from the api
  */
  char * POSTEntityPhone( char *entity_id, char *number, char *description, char *trackable) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"number=");
    p = curl_easy_escape(curl,number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"trackable=");
    p = curl_easy_escape(curl,trackable,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/phone",params);
  }


  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityPhone( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/phone",params);
  }


  /**
   * Create/Update a postal address
   *
   *  @param entity_id
   *  @param building_number
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param province
   *  @param postcode
   *  @param address_type
   *  @param do_not_display
   *  @return - the data from the api
  */
  char * POSTEntityPostal_address( char *entity_id, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *address_type, char *do_not_display) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"building_number=");
    p = curl_easy_escape(curl,building_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address1=");
    p = curl_easy_escape(curl,address1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address2=");
    p = curl_easy_escape(curl,address2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address3=");
    p = curl_easy_escape(curl,address3,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"district=");
    p = curl_easy_escape(curl,district,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"town=");
    p = curl_easy_escape(curl,town,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"county=");
    p = curl_easy_escape(curl,county,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"province=");
    p = curl_easy_escape(curl,province,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcode=");
    p = curl_easy_escape(curl,postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address_type=");
    p = curl_easy_escape(curl,address_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"do_not_display=");
    p = curl_easy_escape(curl,do_not_display,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/postal_address",params);
  }


  /**
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID
   *  @return - the data from the api
  */
  char * GETEntityProvisionalBy_supplier_id( char *supplier_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"supplier_id=");
    p = curl_easy_escape(curl,supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/provisional/by_supplier_id",params);
  }


  /**
   * removes a given entities supplier/masheryid/user_id content and makes the entity inactive if the entity is un-usable
   *
   *  @param entity_id - The entity to pull
   *  @param purge_masheryid - The purge masheryid to match
   *  @param purge_supplier_id - The purge supplier id to match
   *  @param purge_user_id - The purge user id to match
   *  @param exclude - List of entity fields that are excluded from the purge
   *  @param destructive
   *  @return - the data from the api
  */
  char * POSTEntityPurge( char *entity_id, char *purge_masheryid, char *purge_supplier_id, char *purge_user_id, char *exclude, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"purge_masheryid=");
    p = curl_easy_escape(curl,purge_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"purge_supplier_id=");
    p = curl_easy_escape(curl,purge_supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"purge_user_id=");
    p = curl_easy_escape(curl,purge_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"exclude=");
    p = curl_easy_escape(curl,exclude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/purge",params);
  }


  /**
   * removes a portion of a given entity and makes the entity inactive if the resulting leftover entity is un-usable
   *
   *  @param entity_id - The entity to pull
   *  @param object
   *  @param gen_id - The gen_id of any multi-object being purged
   *  @param purge_masheryid - The purge masheryid to match
   *  @param purge_supplier_id - The purge supplier id to match
   *  @param purge_user_id - The purge user id to match
   *  @param destructive
   *  @return - the data from the api
  */
  char * POSTEntityPurgeBy_object( char *entity_id, char *object, char *gen_id, char *purge_masheryid, char *purge_supplier_id, char *purge_user_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"object=");
    p = curl_easy_escape(curl,object,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"purge_masheryid=");
    p = curl_easy_escape(curl,purge_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"purge_supplier_id=");
    p = curl_easy_escape(curl,purge_supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"purge_user_id=");
    p = curl_easy_escape(curl,purge_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/purge/by_object",params);
  }


  /**
   * Deletes a specific review for an entity via Review API
   *
   *  @param entity_id - The entity with the review
   *  @param review_id - The review id
   *  @return - the data from the api
  */
  char * DELETEEntityReview( char *entity_id, char *review_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"review_id=");
    p = curl_easy_escape(curl,review_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/review",params);
  }


  /**
   * Gets a specific review  for an entity
   *
   *  @param entity_id - The entity with the review
   *  @param review_id - The review id
   *  @return - the data from the api
  */
  char * GETEntityReview( char *entity_id, char *review_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"review_id=");
    p = curl_easy_escape(curl,review_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/review",params);
  }


  /**
   * Appends a review to an entity
   *
   *  @param entity_id - the entity to append the review to
   *  @param reviewer_user_id - The user id
   *  @param review_id - The review id. If this is supplied will attempt to update an existing review
   *  @param title - The title of the review
   *  @param content - The full text content of the review
   *  @param star_rating - The rating of the review
   *  @param domain - The domain the review originates from
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTEntityReview( char *entity_id, char *reviewer_user_id, char *review_id, char *title, char *content, char *star_rating, char *domain, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reviewer_user_id=");
    p = curl_easy_escape(curl,reviewer_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"review_id=");
    p = curl_easy_escape(curl,review_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"title=");
    p = curl_easy_escape(curl,title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"content=");
    p = curl_easy_escape(curl,content,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"star_rating=");
    p = curl_easy_escape(curl,star_rating,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/review",params);
  }


  /**
   * Gets all reviews for an entity
   *
   *  @param entity_id - The entity with the review
   *  @param limit - Limit the number of results returned
   *  @param skip - Number of results skipped
   *  @return - the data from the api
  */
  char * GETEntityReviewList( char *entity_id, char *limit, char *skip) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip=");
    p = curl_easy_escape(curl,skip,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/review/list",params);
  }


  /**
   * Allows a list of available revisions to be returned by its entity id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETEntityRevisions( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/revisions",params);
  }


  /**
   * Allows a specific revision of an entity to be returned by entity id and a revision number
   *
   *  @param entity_id
   *  @param revision_id
   *  @return - the data from the api
  */
  char * GETEntityRevisionsByRevisionID( char *entity_id, char *revision_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"revision_id=");
    p = curl_easy_escape(curl,revision_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/revisions/byRevisionID",params);
  }


  /**
   * Search for matching entities
   *
   *  @param latitude_1
   *  @param longitude_1
   *  @param latitude_2
   *  @param longitude_2
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page
   *  @param page
   *  @param country
   *  @param language
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @return - the data from the api
  */
  char * GETEntitySearchByboundingbox( char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"latitude_1=");
    p = curl_easy_escape(curl,latitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_1=");
    p = curl_easy_escape(curl,longitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_2=");
    p = curl_easy_escape(curl,latitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_2=");
    p = curl_easy_escape(curl,longitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/byboundingbox",params);
  }


  /**
   * Search for matching entities
   *
   *  @param where - Location to search for results. E.g. Dublin e.g. Dublin
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page - How many results per page
   *  @param page - What page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the search context (optional)
   *  @param longitude - The decimal longitude of the search context (optional)
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @param benchmark
   *  @return - the data from the api
  */
  char * GETEntitySearchBylocation( char *where, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids, char *benchmark) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"benchmark=");
    p = curl_easy_escape(curl,benchmark,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/bylocation",params);
  }


  /**
   * Search for entities matching the supplied group_id, ordered by nearness
   *
   *  @param group_id - the group_id to search for
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param country - The country to fetch results for e.g. gb
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the centre point of the search
   *  @param longitude - The decimal longitude of the centre point of the search
   *  @param where - The location to search for
   *  @param domain
   *  @param path
   *  @param unitOfDistance
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @return - the data from the api
  */
  char * GETEntitySearchGroupBynearest( char *group_id, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *where, char *domain, char *path, char *unitOfDistance, char *restrict_category_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"unitOfDistance=");
    p = curl_easy_escape(curl,unitOfDistance,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/group/bynearest",params);
  }


  /**
   * Search for entities matching the supplied 'who', ordered by nearness. NOTE if you want to see any advertisers then append MASHERYID (even if using API key) and include_ads=true to get your ads matching that keyword and the derived location.
   *
   *  @param keyword - What to get results for. E.g. cafe e.g. cafe
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param country - The country to fetch results for e.g. gb
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the centre point of the search
   *  @param longitude - The decimal longitude of the centre point of the search
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @param include_ads - Find nearby advertisers with tags that match the keyword
   *  @return - the data from the api
  */
  char * GETEntitySearchKeywordBynearest( char *keyword, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids, char *include_ads) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"keyword=");
    p = curl_easy_escape(curl,keyword,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"include_ads=");
    p = curl_easy_escape(curl,include_ads,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/keyword/bynearest",params);
  }


  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page - Number of results returned per page
   *  @param page - The page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @param benchmark
   *  @return - the data from the api
  */
  char * GETEntitySearchWhat( char *what, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids, char *benchmark) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"what=");
    p = curl_easy_escape(curl,what,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"benchmark=");
    p = curl_easy_escape(curl,benchmark,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/what",params);
  }


  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param latitude_1 - Latitude of first point in bounding box e.g. 53.396842
   *  @param longitude_1 - Longitude of first point in bounding box e.g. -6.37619
   *  @param latitude_2 - Latitude of second point in bounding box e.g. 53.290463
   *  @param longitude_2 - Longitude of second point in bounding box e.g. -6.207275
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page
   *  @param page
   *  @param country - A valid ISO 3166 country code e.g. ie
   *  @param language
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @return - the data from the api
  */
  char * GETEntitySearchWhatByboundingbox( char *what, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"what=");
    p = curl_easy_escape(curl,what,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_1=");
    p = curl_easy_escape(curl,latitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_1=");
    p = curl_easy_escape(curl,longitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_2=");
    p = curl_easy_escape(curl,latitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_2=");
    p = curl_easy_escape(curl,longitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/what/byboundingbox",params);
  }


  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param where - The location to get results for. E.g. Dublin e.g. Dublin
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the search context (optional)
   *  @param longitude - The decimal longitude of the search context (optional)
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @param benchmark
   *  @return - the data from the api
  */
  char * GETEntitySearchWhatBylocation( char *what, char *where, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids, char *benchmark) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"what=");
    p = curl_easy_escape(curl,what,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"benchmark=");
    p = curl_easy_escape(curl,benchmark,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/what/bylocation",params);
  }


  /**
   * Search for matching entities, ordered by nearness
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param country - The country to fetch results for e.g. gb
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the centre point of the search
   *  @param longitude - The decimal longitude of the centre point of the search
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @return - the data from the api
  */
  char * GETEntitySearchWhatBynearest( char *what, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"what=");
    p = curl_easy_escape(curl,what,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/what/bynearest",params);
  }


  /**
   * Search for matching entities
   *
   *  @param who - Company name e.g. Starbucks
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page - How many results per page
   *  @param page - What page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @param benchmark
   *  @return - the data from the api
  */
  char * GETEntitySearchWho( char *who, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids, char *benchmark) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"who=");
    p = curl_easy_escape(curl,who,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"benchmark=");
    p = curl_easy_escape(curl,benchmark,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/who",params);
  }


  /**
   * Search for matching entities
   *
   *  @param who
   *  @param latitude_1
   *  @param longitude_1
   *  @param latitude_2
   *  @param longitude_2
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page
   *  @param page
   *  @param country
   *  @param language - An ISO compatible language code, E.g. en
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @return - the data from the api
  */
  char * GETEntitySearchWhoByboundingbox( char *who, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"who=");
    p = curl_easy_escape(curl,who,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_1=");
    p = curl_easy_escape(curl,latitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_1=");
    p = curl_easy_escape(curl,longitude_1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude_2=");
    p = curl_easy_escape(curl,latitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude_2=");
    p = curl_easy_escape(curl,longitude_2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/who/byboundingbox",params);
  }


  /**
   * Search for matching entities
   *
   *  @param who - Company Name e.g. Starbucks
   *  @param where - The location to get results for. E.g. Dublin e.g. Dublin
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param latitude - The decimal latitude of the search context (optional)
   *  @param longitude - The decimal longitude of the search context (optional)
   *  @param language - An ISO compatible language code, E.g. en
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @param benchmark
   *  @return - the data from the api
  */
  char * GETEntitySearchWhoBylocation( char *who, char *where, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *latitude, char *longitude, char *language, char *domain, char *path, char *restrict_category_ids, char *benchmark) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"who=");
    p = curl_easy_escape(curl,who,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"benchmark=");
    p = curl_easy_escape(curl,benchmark,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/who/bylocation",params);
  }


  /**
   * Search for entities matching the supplied 'who', ordered by nearness
   *
   *  @param who - What to get results for. E.g. Plumber e.g. plumber
   *  @param orderonline - Favours online ordering where set to true
   *  @param delivers - Favours delivery where set to true
   *  @param isClaimed - 1: claimed; 0: not claimed or claim expired; -1: ignore this filter.
   *  @param country - The country to fetch results for e.g. gb
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the centre point of the search
   *  @param longitude - The decimal longitude of the centre point of the search
   *  @param domain
   *  @param path
   *  @param restrict_category_ids - Pipe delimited optional IDs to restrict matches to (optional)
   *  @return - the data from the api
  */
  char * GETEntitySearchWhoBynearest( char *who, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"who=");
    p = curl_easy_escape(curl,who,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"orderonline=");
    p = curl_easy_escape(curl,orderonline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"delivers=");
    p = curl_easy_escape(curl,delivers,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"isClaimed=");
    p = curl_easy_escape(curl,isClaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"restrict_category_ids=");
    p = curl_easy_escape(curl,restrict_category_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/search/who/bynearest",params);
  }


  /**
   * Send an email to an email address specified in an entity
   *
   *  @param entity_id - The entity id of the entity you wish to contact
   *  @param gen_id - The gen_id of the email address you wish to send the message to
   *  @param from_email - The email of the person sending the message 
   *  @param subject - The subject for the email
   *  @param content - The content of the email
   *  @return - the data from the api
  */
  char * POSTEntitySend_email( char *entity_id, char *gen_id, char *from_email, char *subject, char *content) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_email=");
    p = curl_easy_escape(curl,from_email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"subject=");
    p = curl_easy_escape(curl,subject,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"content=");
    p = curl_easy_escape(curl,content,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/send_email",params);
  }


  /**
   * With a known entity id, a service object can be added.
   *
   *  @param entity_id
   *  @param value
   *  @return - the data from the api
  */
  char * POSTEntityService( char *entity_id, char *value) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"value=");
    p = curl_easy_escape(curl,value,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/service",params);
  }


  /**
   * With a known entity id, a service object can be deleted.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityService( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/service",params);
  }


  /**
   * With a known entity id, a social media object can be added.
   *
   *  @param entity_id
   *  @param type
   *  @param website_url
   *  @return - the data from the api
  */
  char * POSTEntitySocialmedia( char *entity_id, char *type, char *website_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"website_url=");
    p = curl_easy_escape(curl,website_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/socialmedia",params);
  }


  /**
   * Allows a social media object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntitySocialmedia( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/socialmedia",params);
  }


  /**
   * Allows a special offer object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntitySpecial_offer( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/special_offer",params);
  }


  /**
   * With a known entity id, a website object can be added.
   *
   *  @param entity_id
   *  @param title
   *  @param description
   *  @param terms
   *  @param start_date
   *  @param expiry_date
   *  @param url
   *  @return - the data from the api
  */
  char * POSTEntitySpecial_offer( char *entity_id, char *title, char *description, char *terms, char *start_date, char *expiry_date, char *url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"title=");
    p = curl_easy_escape(curl,title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"terms=");
    p = curl_easy_escape(curl,terms,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"start_date=");
    p = curl_easy_escape(curl,start_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"url=");
    p = curl_easy_escape(curl,url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/special_offer",params);
  }


  /**
   * With a known entity id, a status object can be updated.
   *
   *  @param entity_id
   *  @param status
   *  @param inactive_reason
   *  @param inactive_description
   *  @return - the data from the api
  */
  char * POSTEntityStatus( char *entity_id, char *status, char *inactive_reason, char *inactive_description) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"status=");
    p = curl_easy_escape(curl,status,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inactive_reason=");
    p = curl_easy_escape(curl,inactive_reason,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inactive_description=");
    p = curl_easy_escape(curl,inactive_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/status",params);
  }


  /**
   * Suspend all entiies added or claimed by a specific user
   *
   *  @param user_id - The unique user ID of the user with claimed entities e.g. 379236608286720
   *  @return - the data from the api
  */
  char * POSTEntityStatusSuspend_by_user_id( char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/status/suspend_by_user_id",params);
  }


  /**
   * Allows a tag object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityTag( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/tag",params);
  }


  /**
   * With a known entity id, an tag object can be added.
   *
   *  @param entity_id
   *  @param tag
   *  @param language
   *  @return - the data from the api
  */
  char * POSTEntityTag( char *entity_id, char *tag, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tag=");
    p = curl_easy_escape(curl,tag,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/tag",params);
  }


  /**
   * Allows a testimonial object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityTestimonial( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/testimonial",params);
  }


  /**
   * With a known entity id, a testimonial object can be added.
   *
   *  @param entity_id
   *  @param title
   *  @param text
   *  @param date
   *  @param testifier_name
   *  @return - the data from the api
  */
  char * POSTEntityTestimonial( char *entity_id, char *title, char *text, char *date, char *testifier_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"title=");
    p = curl_easy_escape(curl,title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"text=");
    p = curl_easy_escape(curl,text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"date=");
    p = curl_easy_escape(curl,date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"testifier_name=");
    p = curl_easy_escape(curl,testifier_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/testimonial",params);
  }


  /**
   * Get the updates a uncontribute would perform
   *
   *  @param entity_id - The entity to pull
   *  @param object_name - The entity object to update
   *  @param supplier_id - The supplier_id to remove
   *  @param user_id - The user_id to remove
   *  @return - the data from the api
  */
  char * GETEntityUncontribute( char *entity_id, char *object_name, char *supplier_id, char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"object_name=");
    p = curl_easy_escape(curl,object_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_id=");
    p = curl_easy_escape(curl,supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/uncontribute",params);
  }


  /**
   * Separates an entity into two distinct entities 
   *
   *  @param entity_id
   *  @param unmerge_masheryid
   *  @param unmerge_supplier_id
   *  @param unmerge_user_id
   *  @param destructive
   *  @return - the data from the api
  */
  char * POSTEntityUnmerge( char *entity_id, char *unmerge_masheryid, char *unmerge_supplier_id, char *unmerge_user_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"unmerge_masheryid=");
    p = curl_easy_escape(curl,unmerge_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"unmerge_supplier_id=");
    p = curl_easy_escape(curl,unmerge_supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"unmerge_user_id=");
    p = curl_easy_escape(curl,unmerge_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/unmerge",params);
  }


  /**
   * Find the provided user in all the sub objects and update the trust
   *
   *  @param entity_id - the entity_id to update
   *  @param user_id - the user to search for
   *  @param trust - The new trust for the user
   *  @return - the data from the api
  */
  char * POSTEntityUser_trust( char *entity_id, char *user_id, char *trust) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"trust=");
    p = curl_easy_escape(curl,trust,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/user_trust",params);
  }


  /**
   * Add a verified source object to an existing entity.
   *
   *  @param entity_id
   *  @param public_source - Corresponds to entity_obj.attribution.name
   *  @param source_name - Corresponds to entity_obj.data_sources.type
   *  @param source_id - Corresponds to entity_obj.data_sources.external_id
   *  @param source_url - Corresponds to entity_obj.attribution.url
   *  @param source_logo - Corresponds to entity_obj.attribution.logo
   *  @param verified_date - Corresponds to entity_obj.data_sources.created_at
   *  @return - the data from the api
  */
  char * POSTEntityVerified( char *entity_id, char *public_source, char *source_name, char *source_id, char *source_url, char *source_logo, char *verified_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"public_source=");
    p = curl_easy_escape(curl,public_source,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source_name=");
    p = curl_easy_escape(curl,source_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source_id=");
    p = curl_easy_escape(curl,source_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source_url=");
    p = curl_easy_escape(curl,source_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source_logo=");
    p = curl_easy_escape(curl,source_logo,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"verified_date=");
    p = curl_easy_escape(curl,verified_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/verified",params);
  }


  /**
   * Remove a verified source object to an existing entity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityVerified( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/verified",params);
  }


  /**
   * With a known entity id, a video object can be added.
   *
   *  @param entity_id
   *  @param type
   *  @param link
   *  @return - the data from the api
  */
  char * POSTEntityVideo( char *entity_id, char *type, char *link) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"link=");
    p = curl_easy_escape(curl,link,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/video",params);
  }


  /**
   * Allows a video object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityVideo( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/video",params);
  }


  /**
   * With a known entity id, a YouTube video object can be added.
   *
   *  @param entity_id
   *  @param embed_code
   *  @return - the data from the api
  */
  char * POSTEntityVideoYoutube( char *entity_id, char *embed_code) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"embed_code=");
    p = curl_easy_escape(curl,embed_code,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/video/youtube",params);
  }


  /**
   * Allows a website object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @param force
   *  @return - the data from the api
  */
  char * DELETEEntityWebsite( char *entity_id, char *gen_id, char *force) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"force=");
    p = curl_easy_escape(curl,force,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/website",params);
  }


  /**
   * With a known entity id, a website object can be added.
   *
   *  @param entity_id
   *  @param website_url
   *  @param display_url
   *  @param website_description
   *  @param gen_id
   *  @return - the data from the api
  */
  char * POSTEntityWebsite( char *entity_id, char *website_url, char *display_url, char *website_description, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"website_url=");
    p = curl_easy_escape(curl,website_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"display_url=");
    p = curl_easy_escape(curl,display_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"website_description=");
    p = curl_easy_escape(curl,website_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/website",params);
  }


  /**
   * With a known entity id, a yext list can be added
   *
   *  @param entity_id
   *  @param yext_list_id
   *  @param description
   *  @param name
   *  @param type
   *  @return - the data from the api
  */
  char * POSTEntityYext_list( char *entity_id, char *yext_list_id, char *description, char *name, char *type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"yext_list_id=");
    p = curl_easy_escape(curl,yext_list_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/yext_list",params);
  }


  /**
   * Allows a yext list object to be removed
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityYext_list( char *entity_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/entity/yext_list",params);
  }


  /**
   * Add an entityserve document
   *
   *  @param entity_id - The ids of the entity/entities to create the entityserve event(s) for
   *  @param country - the ISO code of the country
   *  @param event_type - The event type being recorded
   *  @param domain
   *  @param path
   *  @return - the data from the api
  */
  char * PUTEntityserve( char *entity_id, char *country, char *event_type, char *domain, char *path) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"event_type=");
    p = curl_easy_escape(curl,event_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/entityserve",params);
  }


  /**
   * Update/Add a flatpack
   *
   *  @param flatpack_id - this record's unique, auto-generated id - if supplied, then this is an edit, otherwise it's an add
   *  @param status - The status of the flatpack, it is required on creation. Syndication link logic depends on this.
   *  @param nodefaults - create an flatpack that's empty apart from provided values (used for child flatpacks), IMPORTANT: if set, any parameters with default values will be ignored even if overridden. Edit the created flatpack to set those parameters on a nodefaults flatpack.
   *  @param domainName - the domain name to serve this flatpack site on (no leading http:// or anything please)
   *  @param inherits - inherit from domain
   *  @param stub - the stub that is appended to the flatpack's url e.g. http://dev.localhost/stub
   *  @param flatpackName - the name of the Flat pack instance
   *  @param less - the LESS configuration to use to overrides the Bootstrap CSS
   *  @param language - the language in which to render the flatpack site
   *  @param country - the country to use for searches etc
   *  @param mapsType - the type of maps to use
   *  @param mapKey - the nokia map key to use to render maps
   *  @param searchFormShowOn - list of pages to show the search form
   *  @param searchFormShowKeywordsBox - whether to display the keywords box on the search form
   *  @param searchFormShowLocationBox - whether to display the location box on search forms - not required
   *  @param searchFormKeywordsAutoComplete - whether to do auto-completion on the keywords box on the search form
   *  @param searchFormLocationsAutoComplete - whether to do auto-completion on the locations box on the search form
   *  @param searchFormDefaultLocation - the string to use as the default location for searches if no location is supplied
   *  @param searchFormPlaceholderKeywords - the string to show in the keyword box as placeholder text e.g. e.g. cafe
   *  @param searchFormPlaceholderLocation - the string to show in the location box as placeholder text e.g. e.g. Dublin
   *  @param searchFormKeywordsLabel - the string to show next to the keywords control e.g. I'm looking for
   *  @param searchFormLocationLabel - the string to show next to the location control e.g. Located in
   *  @param cannedLinksHeader - the string to show above canned searches
   *  @param homepageTitle - the page title of site's home page
   *  @param homepageDescription - the meta description of the home page
   *  @param homepageIntroTitle - the introductory title for the homepage
   *  @param homepageIntroText - the introductory text for the homepage
   *  @param head - payload to put in the head of the flatpack
   *  @param adblock - payload to put in the adblock of the flatpack
   *  @param bodyTop - the payload to put in the top of the body of a flatpack
   *  @param bodyBottom - the payload to put in the bottom of the body of a flatpack
   *  @param header_menu - the JSON that describes a navigation at the top of the page
   *  @param header_menu_bottom - the JSON that describes a navigation below the masthead
   *  @param footer_menu - the JSON that describes a navigation at the bottom of the page
   *  @param bdpTitle - The page title of the entity business profile pages
   *  @param bdpDescription - The meta description of entity business profile pages
   *  @param bdpAds - The block of HTML/JS that renders Ads on BDPs
   *  @param serpTitle - The page title of the serps
   *  @param serpDescription - The meta description of serps
   *  @param serpNumberResults - The number of results per search page
   *  @param serpNumberAdverts - The number of adverts to show on the first search page
   *  @param serpAds - The block of HTML/JS that renders Ads on Serps
   *  @param serpAdsBottom - The block of HTML/JS that renders Ads on Serps at the bottom
   *  @param serpTitleNoWhat - The text to display in the title for where only searches
   *  @param serpDescriptionNoWhat - The text to display in the description for where only searches
   *  @param cookiePolicyUrl - The cookie policy url of the flatpack
   *  @param cookiePolicyNotice - Whether to show the cookie policy on this flatpack
   *  @param addBusinessButtonText - The text used in the 'Add your business' button
   *  @param twitterUrl - Twitter link
   *  @param facebookUrl - Facebook link
   *  @param copyright - Copyright message
   *  @param phoneReveal - record phone number reveal
   *  @param loginLinkText - the link text for the Login link
   *  @param contextLocationId - The location ID to use as the context for searches on this flatpack
   *  @param addBusinessButtonPosition - The location ID to use as the context for searches on this flatpack
   *  @param denyIndexing - Whether to noindex a flatpack
   *  @param contextRadius - allows you to set a catchment area around the contextLocationId in miles for use when displaying the activity stream module
   *  @param activityStream - allows you to set the activity to be displayed in the activity stream
   *  @param activityStreamSize - Sets the number of items to show within the activity stream.
   *  @param products - A Collection of Central Index products the flatpack is allowed to sell
   *  @param linkToRoot - The root domain name to serve this flatpack site on (no leading http:// or anything please)
   *  @param termsLink - A URL for t's and c's specific to this partner
   *  @param serpNumberEmbedAdverts - The number of embed adverts per search
   *  @param serpEmbedTitle - Custom page title for emdedded searches
   *  @param adminLess - the LESS configuration to use to overrides the Bootstrap CSS for the admin on themed domains
   *  @param adminConfNoLocz - operate without recourse to verified location data (locz)
   *  @param adminConfNoSocialLogin - suppress social media login interface
   *  @param adminConfEasyClaim - captcha only for claim
   *  @param adminConfPaymentMode - payment gateway
   *  @param adminConfEnableProducts - show upgrade on claim
   *  @param adminConfSimpleadmin - render a template for the reduced functionality
   *  @return - the data from the api
  */
  char * POSTFlatpack( char *flatpack_id, char *status, char *nodefaults, char *domainName, char *inherits, char *stub, char *flatpackName, char *less, char *language, char *country, char *mapsType, char *mapKey, char *searchFormShowOn, char *searchFormShowKeywordsBox, char *searchFormShowLocationBox, char *searchFormKeywordsAutoComplete, char *searchFormLocationsAutoComplete, char *searchFormDefaultLocation, char *searchFormPlaceholderKeywords, char *searchFormPlaceholderLocation, char *searchFormKeywordsLabel, char *searchFormLocationLabel, char *cannedLinksHeader, char *homepageTitle, char *homepageDescription, char *homepageIntroTitle, char *homepageIntroText, char *head, char *adblock, char *bodyTop, char *bodyBottom, char *header_menu, char *header_menu_bottom, char *footer_menu, char *bdpTitle, char *bdpDescription, char *bdpAds, char *serpTitle, char *serpDescription, char *serpNumberResults, char *serpNumberAdverts, char *serpAds, char *serpAdsBottom, char *serpTitleNoWhat, char *serpDescriptionNoWhat, char *cookiePolicyUrl, char *cookiePolicyNotice, char *addBusinessButtonText, char *twitterUrl, char *facebookUrl, char *copyright, char *phoneReveal, char *loginLinkText, char *contextLocationId, char *addBusinessButtonPosition, char *denyIndexing, char *contextRadius, char *activityStream, char *activityStreamSize, char *products, char *linkToRoot, char *termsLink, char *serpNumberEmbedAdverts, char *serpEmbedTitle, char *adminLess, char *adminConfNoLocz, char *adminConfNoSocialLogin, char *adminConfEasyClaim, char *adminConfPaymentMode, char *adminConfEnableProducts, char *adminConfSimpleadmin) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"status=");
    p = curl_easy_escape(curl,status,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"nodefaults=");
    p = curl_easy_escape(curl,nodefaults,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inherits=");
    p = curl_easy_escape(curl,inherits,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"stub=");
    p = curl_easy_escape(curl,stub,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpackName=");
    p = curl_easy_escape(curl,flatpackName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"less=");
    p = curl_easy_escape(curl,less,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapsType=");
    p = curl_easy_escape(curl,mapsType,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapKey=");
    p = curl_easy_escape(curl,mapKey,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormShowOn=");
    p = curl_easy_escape(curl,searchFormShowOn,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormShowKeywordsBox=");
    p = curl_easy_escape(curl,searchFormShowKeywordsBox,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormShowLocationBox=");
    p = curl_easy_escape(curl,searchFormShowLocationBox,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormKeywordsAutoComplete=");
    p = curl_easy_escape(curl,searchFormKeywordsAutoComplete,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormLocationsAutoComplete=");
    p = curl_easy_escape(curl,searchFormLocationsAutoComplete,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormDefaultLocation=");
    p = curl_easy_escape(curl,searchFormDefaultLocation,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormPlaceholderKeywords=");
    p = curl_easy_escape(curl,searchFormPlaceholderKeywords,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormPlaceholderLocation=");
    p = curl_easy_escape(curl,searchFormPlaceholderLocation,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormKeywordsLabel=");
    p = curl_easy_escape(curl,searchFormKeywordsLabel,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchFormLocationLabel=");
    p = curl_easy_escape(curl,searchFormLocationLabel,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"cannedLinksHeader=");
    p = curl_easy_escape(curl,cannedLinksHeader,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"homepageTitle=");
    p = curl_easy_escape(curl,homepageTitle,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"homepageDescription=");
    p = curl_easy_escape(curl,homepageDescription,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"homepageIntroTitle=");
    p = curl_easy_escape(curl,homepageIntroTitle,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"homepageIntroText=");
    p = curl_easy_escape(curl,homepageIntroText,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"head=");
    p = curl_easy_escape(curl,head,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adblock=");
    p = curl_easy_escape(curl,adblock,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bodyTop=");
    p = curl_easy_escape(curl,bodyTop,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bodyBottom=");
    p = curl_easy_escape(curl,bodyBottom,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"header_menu=");
    p = curl_easy_escape(curl,header_menu,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"header_menu_bottom=");
    p = curl_easy_escape(curl,header_menu_bottom,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"footer_menu=");
    p = curl_easy_escape(curl,footer_menu,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bdpTitle=");
    p = curl_easy_escape(curl,bdpTitle,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bdpDescription=");
    p = curl_easy_escape(curl,bdpDescription,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bdpAds=");
    p = curl_easy_escape(curl,bdpAds,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpTitle=");
    p = curl_easy_escape(curl,serpTitle,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpDescription=");
    p = curl_easy_escape(curl,serpDescription,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpNumberResults=");
    p = curl_easy_escape(curl,serpNumberResults,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpNumberAdverts=");
    p = curl_easy_escape(curl,serpNumberAdverts,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpAds=");
    p = curl_easy_escape(curl,serpAds,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpAdsBottom=");
    p = curl_easy_escape(curl,serpAdsBottom,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpTitleNoWhat=");
    p = curl_easy_escape(curl,serpTitleNoWhat,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpDescriptionNoWhat=");
    p = curl_easy_escape(curl,serpDescriptionNoWhat,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"cookiePolicyUrl=");
    p = curl_easy_escape(curl,cookiePolicyUrl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"cookiePolicyNotice=");
    p = curl_easy_escape(curl,cookiePolicyNotice,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"addBusinessButtonText=");
    p = curl_easy_escape(curl,addBusinessButtonText,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"twitterUrl=");
    p = curl_easy_escape(curl,twitterUrl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"facebookUrl=");
    p = curl_easy_escape(curl,facebookUrl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"copyright=");
    p = curl_easy_escape(curl,copyright,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"phoneReveal=");
    p = curl_easy_escape(curl,phoneReveal,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"loginLinkText=");
    p = curl_easy_escape(curl,loginLinkText,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"contextLocationId=");
    p = curl_easy_escape(curl,contextLocationId,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"addBusinessButtonPosition=");
    p = curl_easy_escape(curl,addBusinessButtonPosition,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"denyIndexing=");
    p = curl_easy_escape(curl,denyIndexing,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"contextRadius=");
    p = curl_easy_escape(curl,contextRadius,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"activityStream=");
    p = curl_easy_escape(curl,activityStream,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"activityStreamSize=");
    p = curl_easy_escape(curl,activityStreamSize,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"products=");
    p = curl_easy_escape(curl,products,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"linkToRoot=");
    p = curl_easy_escape(curl,linkToRoot,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"termsLink=");
    p = curl_easy_escape(curl,termsLink,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpNumberEmbedAdverts=");
    p = curl_easy_escape(curl,serpNumberEmbedAdverts,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpEmbedTitle=");
    p = curl_easy_escape(curl,serpEmbedTitle,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminLess=");
    p = curl_easy_escape(curl,adminLess,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminConfNoLocz=");
    p = curl_easy_escape(curl,adminConfNoLocz,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminConfNoSocialLogin=");
    p = curl_easy_escape(curl,adminConfNoSocialLogin,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminConfEasyClaim=");
    p = curl_easy_escape(curl,adminConfEasyClaim,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminConfPaymentMode=");
    p = curl_easy_escape(curl,adminConfPaymentMode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminConfEnableProducts=");
    p = curl_easy_escape(curl,adminConfEnableProducts,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adminConfSimpleadmin=");
    p = curl_easy_escape(curl,adminConfSimpleadmin,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack",params);
  }


  /**
   * Get a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETFlatpack( char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack",params);
  }


  /**
   * Remove a flatpack using a supplied flatpack_id
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @return - the data from the api
  */
  char * DELETEFlatpack( char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/flatpack",params);
  }


  /**
   * Upload a CSS file for the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminCSS( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/adminCSS",params);
  }


  /**
   * Add a HD Admin logo to a flatpack domain
   *
   *  @param flatpack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminHDLogo( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/adminHDLogo",params);
  }


  /**
   * Upload an image to serve out as the large logo in the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminLargeLogo( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/adminLargeLogo",params);
  }


  /**
   * Upload an image to serve out as the small logo in the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminSmallLogo( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/adminSmallLogo",params);
  }


  /**
   * Remove a flatpack using a supplied flatpack_id
   *
   *  @param flatpack_id - the unique id to search for
   *  @param adblock
   *  @param serpAds
   *  @param serpAdsBottom
   *  @param bdpAds
   *  @return - the data from the api
  */
  char * DELETEFlatpackAds( char *flatpack_id, char *adblock, char *serpAds, char *serpAdsBottom, char *bdpAds) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adblock=");
    p = curl_easy_escape(curl,adblock,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpAds=");
    p = curl_easy_escape(curl,serpAds,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"serpAdsBottom=");
    p = curl_easy_escape(curl,serpAdsBottom,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bdpAds=");
    p = curl_easy_escape(curl,bdpAds,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/flatpack/ads",params);
  }


  /**
   * Generate flatpacks based on the files passed in
   *
   *  @param json - The flatpack JSON to make replacements on
   *  @param filedata - a file that contains the replacements in the JSON
   *  @param slack_user
   *  @return - the data from the api
  */
  char * POSTFlatpackBulkJson( char *json, char *filedata, char *slack_user) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"json=");
    p = curl_easy_escape(curl,json,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"slack_user=");
    p = curl_easy_escape(curl,slack_user,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/bulk/json",params);
  }


  /**
   * Get flatpacks by country and location
   *
   *  @param country
   *  @param latitude
   *  @param longitude
   *  @return - the data from the api
  */
  char * GETFlatpackBy_country( char *country, char *latitude, char *longitude) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/by_country",params);
  }


  /**
   * Get flatpacks by country in KML format
   *
   *  @param country
   *  @return - the data from the api
  */
  char * GETFlatpackBy_countryKml( char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/by_country/kml",params);
  }


  /**
   * Get a flatpack using a domain name
   *
   *  @param domainName - the domain name to search for
   *  @param matchAlias - Whether to match alias as well
   *  @return - the data from the api
  */
  char * GETFlatpackBy_domain_name( char *domainName, char *matchAlias) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"matchAlias=");
    p = curl_easy_escape(curl,matchAlias,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/by_domain_name",params);
  }


  /**
   * Get flatpacks that match the supplied masheryid
   *
   *  @return - the data from the api
  */
  char * GETFlatpackBy_masheryid() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/flatpack/by_masheryid",params);
  }


  /**
   * Clone an existing flatpack
   *
   *  @param flatpack_id - the flatpack_id to clone
   *  @param domainName - the domain of the new flatpack site (no leading http:// or anything please)
   *  @return - the data from the api
  */
  char * GETFlatpackClone( char *flatpack_id, char *domainName) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/clone",params);
  }


  /**
   * undefined
   *
   *  @param flatpack_id - the unique id to search for
   *  @param domainName
   *  @return - the data from the api
  */
  char * POSTFlatpackDomain_alias( char *flatpack_id, char *domainName) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/domain_alias",params);
  }


  /**
   * undefined
   *
   *  @param flatpack_id - the unique id to search for
   *  @param domainName
   *  @return - the data from the api
  */
  char * DELETEFlatpackDomain_alias( char *flatpack_id, char *domainName) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/flatpack/domain_alias",params);
  }


  /**
   * Returns a list of domain names in which direct/inherited flatpack country match the specified one and status equals production.
   *
   *  @param country
   *  @return - the data from the api
  */
  char * GETFlatpackDomain_nameBy_country( char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/domain_name/by_country",params);
  }


  /**
   * Upload an icon to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackIcon( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/icon",params);
  }


  /**
   * Get a flatpack using a domain name
   *
   *  @param flatpack_id - the id to search for
   *  @return - the data from the api
  */
  char * GETFlatpackInherit( char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/inherit",params);
  }


  /**
   * returns the LESS theme from a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETFlatpackLess( char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/flatpack/less",params);
  }


  /**
   * Remove a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param gen_id - the id of the canned link to remove
   *  @return - the data from the api
  */
  char * DELETEFlatpackLink( char *flatpack_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/flatpack/link",params);
  }


  /**
   * Add a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param keywords - the keywords to use in the canned search
   *  @param location - the location to use in the canned search
   *  @param linkText - the link text to be used to in the canned search link
   *  @return - the data from the api
  */
  char * POSTFlatpackLink( char *flatpack_id, char *keywords, char *location, char *linkText) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"keywords=");
    p = curl_easy_escape(curl,keywords,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location=");
    p = curl_easy_escape(curl,location,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"linkText=");
    p = curl_easy_escape(curl,linkText,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/link",params);
  }


  /**
   * Remove all canned links from an existing flatpack.
   *
   *  @param flatpack_id - the id of the flatpack to remove links from
   *  @return - the data from the api
  */
  char * DELETEFlatpackLinkAll( char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/flatpack/link/all",params);
  }


  /**
   * Upload a logo to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackLogo( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/logo",params);
  }


  /**
   * Add a hd logo to a flatpack domain
   *
   *  @param flatpack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackLogoHd( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/logo/hd",params);
  }


  /**
   * Delete a Redirect link from a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * DELETEFlatpackRedirect( char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/flatpack/redirect",params);
  }


  /**
   * Add a Redirect link to a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @param redirectTo
   *  @return - the data from the api
  */
  char * POSTFlatpackRedirect( char *flatpack_id, char *redirectTo) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"redirectTo=");
    p = curl_easy_escape(curl,redirectTo,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/redirect",params);
  }


  /**
   * Upload a TXT file to act as the sitemap for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackSitemap( char *flatpack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/flatpack/sitemap",params);
  }


  /**
   * Delete a group with a specified group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * DELETEGroup( char *group_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/group",params);
  }


  /**
   * Update/Add a Group
   *
   *  @param group_id
   *  @param name
   *  @param description
   *  @param url
   *  @param stamp_user_id
   *  @param stamp_sql
   *  @return - the data from the api
  */
  char * POSTGroup( char *group_id, char *name, char *description, char *url, char *stamp_user_id, char *stamp_sql) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"url=");
    p = curl_easy_escape(curl,url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"stamp_user_id=");
    p = curl_easy_escape(curl,stamp_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"stamp_sql=");
    p = curl_easy_escape(curl,stamp_sql,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/group",params);
  }


  /**
   * Returns group that matches a given group id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * GETGroup( char *group_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/group",params);
  }


  /**
   * Returns all groups
   *
   *  @return - the data from the api
  */
  char * GETGroupAll() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/group/all",params);
  }


  /**
   * Bulk delete entities from a specified group
   *
   *  @param group_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTGroupBulk_delete( char *group_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/group/bulk_delete",params);
  }


  /**
   * Bulk ingest entities into a specified group
   *
   *  @param group_id
   *  @param filedata
   *  @param category_type
   *  @return - the data from the api
  */
  char * POSTGroupBulk_ingest( char *group_id, char *filedata, char *category_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_type=");
    p = curl_easy_escape(curl,category_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/group/bulk_ingest",params);
  }


  /**
   * Bulk update entities with a specified group
   *
   *  @param group_id
   *  @param data
   *  @return - the data from the api
  */
  char * POSTGroupBulk_update( char *group_id, char *data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/group/bulk_update",params);
  }


  /**
   * Get number of claims today
   *
   *  @param from_date
   *  @param to_date
   *  @param country_id
   *  @return - the data from the api
  */
  char * GETHeartbeatBy_date( char *from_date, char *to_date, char *country_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_date=");
    p = curl_easy_escape(curl,to_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country_id=");
    p = curl_easy_escape(curl,country_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/heartbeat/by_date",params);
  }


  /**
   * Get number of claims today
   *
   *  @param country
   *  @param claim_type
   *  @return - the data from the api
  */
  char * GETHeartbeatTodayClaims( char *country, char *claim_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claim_type=");
    p = curl_easy_escape(curl,claim_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/heartbeat/today/claims",params);
  }


  /**
   * Process a bulk file
   *
   *  @param job_id
   *  @param filedata - A tab separated file for ingest
   *  @return - the data from the api
  */
  char * POSTIngest_file( char *job_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"job_id=");
    p = curl_easy_escape(curl,job_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/ingest_file",params);
  }


  /**
   * Add a ingest job to the collection
   *
   *  @param description
   *  @param category_type
   *  @return - the data from the api
  */
  char * POSTIngest_job( char *description, char *category_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_type=");
    p = curl_easy_escape(curl,category_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/ingest_job",params);
  }


  /**
   * Get an ingest job from the collection
   *
   *  @param job_id
   *  @return - the data from the api
  */
  char * GETIngest_job( char *job_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"job_id=");
    p = curl_easy_escape(curl,job_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ingest_job",params);
  }


  /**
   * Get an ingest log from the collection
   *
   *  @param job_id
   *  @param success
   *  @param errors
   *  @param limit
   *  @param skip
   *  @return - the data from the api
  */
  char * GETIngest_logBy_job_id( char *job_id, char *success, char *errors, char *limit, char *skip) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"job_id=");
    p = curl_easy_escape(curl,job_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"success=");
    p = curl_easy_escape(curl,success,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"errors=");
    p = curl_easy_escape(curl,errors,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip=");
    p = curl_easy_escape(curl,skip,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ingest_log/by_job_id",params);
  }


  /**
   * Check the status of the Ingest queue, and potentially flush it
   *
   *  @param flush
   *  @return - the data from the api
  */
  char * GETIngest_queue( char *flush) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"flush=");
    p = curl_easy_escape(curl,flush,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ingest_queue",params);
  }


  /**
   * Returns entities that do not have claim or advertisers data
   *
   *  @param country_id - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param from_date
   *  @param to_date
   *  @param limit
   *  @param offset
   *  @param reduce - Set true to return the count value only.
   *  @return - the data from the api
  */
  char * GETLeadsAdded( char *country_id, char *from_date, char *to_date, char *limit, char *offset, char *reduce) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country_id=");
    p = curl_easy_escape(curl,country_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_date=");
    p = curl_easy_escape(curl,to_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"offset=");
    p = curl_easy_escape(curl,offset,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reduce=");
    p = curl_easy_escape(curl,reduce,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/leads/added",params);
  }


  /**
   * Returns entities that have advertisers data
   *
   *  @param country_id - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param from_date
   *  @param to_date
   *  @param limit
   *  @param offset
   *  @param reduce - Set true to return the count value only.
   *  @return - the data from the api
  */
  char * GETLeadsAdvertisers( char *country_id, char *from_date, char *to_date, char *limit, char *offset, char *reduce) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country_id=");
    p = curl_easy_escape(curl,country_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_date=");
    p = curl_easy_escape(curl,to_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"offset=");
    p = curl_easy_escape(curl,offset,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reduce=");
    p = curl_easy_escape(curl,reduce,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/leads/advertisers",params);
  }


  /**
   * Returns entities that have claim data
   *
   *  @param country_id - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param from_date
   *  @param to_date
   *  @param limit
   *  @param offset
   *  @param reduce - Set true to return the count value only.
   *  @return - the data from the api
  */
  char * GETLeadsClaimed( char *country_id, char *from_date, char *to_date, char *limit, char *offset, char *reduce) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country_id=");
    p = curl_easy_escape(curl,country_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_date=");
    p = curl_easy_escape(curl,to_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"offset=");
    p = curl_easy_escape(curl,offset,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reduce=");
    p = curl_easy_escape(curl,reduce,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/leads/claimed",params);
  }


  /**
   * Read a location with the supplied ID in the locations reference database.
   *
   *  @param location_id
   *  @return - the data from the api
  */
  char * GETLocation( char *location_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/location",params);
  }


  /**
   * Create/update a new locz document with the supplied ID in the locations reference database.
   *
   *  @param location_id
   *  @param type
   *  @param country
   *  @param language
   *  @param name
   *  @param formal_name
   *  @param resolution
   *  @param population
   *  @param description
   *  @param timezone
   *  @param latitude
   *  @param longitude
   *  @param parent_town
   *  @param parent_county
   *  @param parent_province
   *  @param parent_region
   *  @param parent_neighbourhood
   *  @param parent_district
   *  @param postalcode
   *  @param searchable_id
   *  @param searchable_ids
   *  @return - the data from the api
  */
  char * POSTLocation( char *location_id, char *type, char *country, char *language, char *name, char *formal_name, char *resolution, char *population, char *description, char *timezone, char *latitude, char *longitude, char *parent_town, char *parent_county, char *parent_province, char *parent_region, char *parent_neighbourhood, char *parent_district, char *postalcode, char *searchable_id, char *searchable_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"formal_name=");
    p = curl_easy_escape(curl,formal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"resolution=");
    p = curl_easy_escape(curl,resolution,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"population=");
    p = curl_easy_escape(curl,population,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"timezone=");
    p = curl_easy_escape(curl,timezone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"parent_town=");
    p = curl_easy_escape(curl,parent_town,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"parent_county=");
    p = curl_easy_escape(curl,parent_county,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"parent_province=");
    p = curl_easy_escape(curl,parent_province,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"parent_region=");
    p = curl_easy_escape(curl,parent_region,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"parent_neighbourhood=");
    p = curl_easy_escape(curl,parent_neighbourhood,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"parent_district=");
    p = curl_easy_escape(curl,parent_district,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postalcode=");
    p = curl_easy_escape(curl,postalcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchable_id=");
    p = curl_easy_escape(curl,searchable_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchable_ids=");
    p = curl_easy_escape(curl,searchable_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/location",params);
  }


  /**
   * Given a location_id or a lat/lon, find other locations within the radius
   *
   *  @param location_id
   *  @param latitude
   *  @param longitude
   *  @param radius - Radius in km
   *  @param resolution
   *  @param country
   *  @param num_results
   *  @return - the data from the api
  */
  char * GETLocationContext( char *location_id, char *latitude, char *longitude, char *radius, char *resolution, char *country, char *num_results) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"location_id=");
    p = curl_easy_escape(curl,location_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"radius=");
    p = curl_easy_escape(curl,radius,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"resolution=");
    p = curl_easy_escape(curl,resolution,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"num_results=");
    p = curl_easy_escape(curl,num_results,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/location/context",params);
  }


  /**
   * Read multiple locations with the supplied ID in the locations reference database.
   *
   *  @param location_ids
   *  @return - the data from the api
  */
  char * GETLocationMultiple( char *location_ids) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"location_ids=");
    p = curl_easy_escape(curl,location_ids,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/location/multiple",params);
  }


  /**
   * With a unique login_id a login can be retrieved
   *
   *  @param login_id
   *  @return - the data from the api
  */
  char * GETLogin( char *login_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"login_id=");
    p = curl_easy_escape(curl,login_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/login",params);
  }


  /**
   * Create/Update login details
   *
   *  @param login_id
   *  @param email
   *  @param password
   *  @return - the data from the api
  */
  char * POSTLogin( char *login_id, char *email, char *password) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"login_id=");
    p = curl_easy_escape(curl,login_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"password=");
    p = curl_easy_escape(curl,password,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/login",params);
  }


  /**
   * With a unique login_id a login can be deleted
   *
   *  @param login_id
   *  @return - the data from the api
  */
  char * DELETELogin( char *login_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"login_id=");
    p = curl_easy_escape(curl,login_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/login",params);
  }


  /**
   * With a unique email address a login can be retrieved
   *
   *  @param email
   *  @return - the data from the api
  */
  char * GETLoginBy_email( char *email) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/login/by_email",params);
  }


  /**
   * Verify that a supplied email and password match a users saved login details
   *
   *  @param email
   *  @param password
   *  @return - the data from the api
  */
  char * GETLoginVerify( char *email, char *password) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"password=");
    p = curl_easy_escape(curl,password,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/login/verify",params);
  }


  /**
   * Fetch the project logo, the symbol of the Wolf
   *
   *  @param a
   *  @param b
   *  @param c
   *  @param d
   *  @return - the data from the api
  */
  char * GETLogo( char *a, char *b, char *c, char *d) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"a=");
    p = curl_easy_escape(curl,a,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"b=");
    p = curl_easy_escape(curl,b,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"c=");
    p = curl_easy_escape(curl,c,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"d=");
    p = curl_easy_escape(curl,d,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/logo",params);
  }


  /**
   * Fetch the project logo, the symbol of the Wolf
   *
   *  @param a
   *  @return - the data from the api
  */
  char * PUTLogo( char *a) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"a=");
    p = curl_easy_escape(curl,a,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/logo",params);
  }


  /**
   * Find a category from cache or cloudant depending if it is in the cache
   *
   *  @param string - A string to search against, E.g. Plumbers
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * GETLookupCategory( char *string, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"string=");
    p = curl_easy_escape(curl,string,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/lookup/category",params);
  }


  /**
   * Find a category from a legacy ID or supplier (e.g. bill_moss)
   *
   *  @param id
   *  @param type
   *  @return - the data from the api
  */
  char * GETLookupLegacyCategory( char *id, char *type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/lookup/legacy/category",params);
  }


  /**
   * Find a location from cache or cloudant depending if it is in the cache (locz)
   *
   *  @param string
   *  @param language
   *  @param country
   *  @param latitude
   *  @param longitude
   *  @return - the data from the api
  */
  char * GETLookupLocation( char *string, char *language, char *country, char *latitude, char *longitude) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"string=");
    p = curl_easy_escape(curl,string,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/lookup/location",params);
  }


  /**
   * Returns a list of mashery IDs domain names in which direct/inherited flatpack country match the specified one and status equals production.
   *
   *  @return - the data from the api
  */
  char * GETMasheryidAll() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/masheryid/all",params);
  }


  /**
   * Find all matches by phone number, returning up to 10 matches
   *
   *  @param phone
   *  @param country
   *  @param exclude - Entity ID to exclude from the results
   *  @return - the data from the api
  */
  char * GETMatchByphone( char *phone, char *country, char *exclude) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"phone=");
    p = curl_easy_escape(curl,phone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"exclude=");
    p = curl_easy_escape(curl,exclude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/match/byphone",params);
  }


  /**
   * Perform a match on the two supplied entities, returning the outcome and showing our working
   *
   *  @param primary_entity_id
   *  @param secondary_entity_id
   *  @param return_entities - Should we return the entity documents
   *  @return - the data from the api
  */
  char * GETMatchOftheday( char *primary_entity_id, char *secondary_entity_id, char *return_entities) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"primary_entity_id=");
    p = curl_easy_escape(curl,primary_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"secondary_entity_id=");
    p = curl_easy_escape(curl,secondary_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"return_entities=");
    p = curl_easy_escape(curl,return_entities,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/match/oftheday",params);
  }


  /**
   * Will create a new Matching Instruction or update an existing one
   *
   *  @param entity_id
   *  @param entity_name
   *  @return - the data from the api
  */
  char * POSTMatching_instruction( char *entity_id, char *entity_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_name=");
    p = curl_easy_escape(curl,entity_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/matching_instruction",params);
  }


  /**
   * Delete Matching instruction
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEMatching_instruction( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/matching_instruction",params);
  }


  /**
   * Fetch all available Matching instructions
   *
   *  @param limit
   *  @return - the data from the api
  */
  char * GETMatching_instructionAll( char *limit) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/matching_instruction/all",params);
  }


  /**
   * Create a matching log
   *
   *  @param primary_entity_id
   *  @param secondary_entity_id
   *  @param primary_name
   *  @param secondary_name
   *  @param address_score
   *  @param address_match
   *  @param name_score
   *  @param name_match
   *  @param distance
   *  @param phone_match
   *  @param category_match
   *  @param email_match
   *  @param website_match
   *  @param match
   *  @return - the data from the api
  */
  char * PUTMatching_log( char *primary_entity_id, char *secondary_entity_id, char *primary_name, char *secondary_name, char *address_score, char *address_match, char *name_score, char *name_match, char *distance, char *phone_match, char *category_match, char *email_match, char *website_match, char *match) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"primary_entity_id=");
    p = curl_easy_escape(curl,primary_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"secondary_entity_id=");
    p = curl_easy_escape(curl,secondary_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"primary_name=");
    p = curl_easy_escape(curl,primary_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"secondary_name=");
    p = curl_easy_escape(curl,secondary_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address_score=");
    p = curl_easy_escape(curl,address_score,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address_match=");
    p = curl_easy_escape(curl,address_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name_score=");
    p = curl_easy_escape(curl,name_score,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name_match=");
    p = curl_easy_escape(curl,name_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"distance=");
    p = curl_easy_escape(curl,distance,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"phone_match=");
    p = curl_easy_escape(curl,phone_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_match=");
    p = curl_easy_escape(curl,category_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email_match=");
    p = curl_easy_escape(curl,email_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"website_match=");
    p = curl_easy_escape(curl,website_match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"match=");
    p = curl_easy_escape(curl,match,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/matching_log",params);
  }


  /**
   * With a known user ID add/create the maxclaims blcok
   *
   *  @param user_id
   *  @param contract_id
   *  @param country
   *  @param number
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTMaxclaimsActivate( char *user_id, char *contract_id, char *country, char *number, char *expiry_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"contract_id=");
    p = curl_easy_escape(curl,contract_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"number=");
    p = curl_easy_escape(curl,number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/maxclaims/activate",params);
  }


  /**
   * Fetching a message
   *
   *  @param message_id - The message id to pull the message for
   *  @return - the data from the api
  */
  char * GETMessage( char *message_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"message_id=");
    p = curl_easy_escape(curl,message_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/message",params);
  }


  /**
   * Update/Add a message
   *
   *  @param message_id - Message id to pull
   *  @param ses_id - Aamazon email id
   *  @param from_user_id - User sending the message
   *  @param from_email - Sent from email address
   *  @param to_entity_id - The id of the entity being sent the message
   *  @param to_email - Sent from email address
   *  @param subject - Subject for the message
   *  @param body - Body for the message
   *  @param bounced - If the message bounced
   *  @return - the data from the api
  */
  char * POSTMessage( char *message_id, char *ses_id, char *from_user_id, char *from_email, char *to_entity_id, char *to_email, char *subject, char *body, char *bounced) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"message_id=");
    p = curl_easy_escape(curl,message_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"ses_id=");
    p = curl_easy_escape(curl,ses_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_user_id=");
    p = curl_easy_escape(curl,from_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from_email=");
    p = curl_easy_escape(curl,from_email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_entity_id=");
    p = curl_easy_escape(curl,to_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_email=");
    p = curl_easy_escape(curl,to_email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"subject=");
    p = curl_easy_escape(curl,subject,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bounced=");
    p = curl_easy_escape(curl,bounced,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/message",params);
  }


  /**
   * Fetching messages by ses_id
   *
   *  @param ses_id - The amazon id to pull the message for
   *  @return - the data from the api
  */
  char * GETMessageBy_ses_id( char *ses_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"ses_id=");
    p = curl_easy_escape(curl,ses_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/message/by_ses_id",params);
  }


  /**
   * Update/Add a multipack
   *
   *  @param multipack_id - this record's unique, auto-generated id - if supplied, then this is an edit, otherwise it's an add
   *  @param group_id - the id of the group that this site serves
   *  @param domainName - the domain name to serve this multipack site on (no leading http:// or anything please)
   *  @param multipackName - the name of the Flat pack instance
   *  @param less - the LESS configuration to use to overrides the Bootstrap CSS
   *  @param country - the country to use for searches etc
   *  @param menuTop - the JSON that describes a navigation at the top of the page
   *  @param menuBottom - the JSON that describes a navigation below the masthead
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @param menuFooter - the JSON that describes a navigation at the bottom of the page
   *  @param searchNumberResults - the number of search results per page
   *  @param searchTitle - Title of serps page
   *  @param searchDescription - Description of serps page
   *  @param searchTitleNoWhere - Title when no where is specified
   *  @param searchDescriptionNoWhere - Description of serps page when no where is specified
   *  @param searchIntroHeader - Introductory header
   *  @param searchIntroText - Introductory text
   *  @param searchShowAll - display all search results on one page
   *  @param searchUnitOfDistance - the unit of distance to use for search
   *  @param cookiePolicyShow - whether to show cookie policy
   *  @param cookiePolicyUrl - url of cookie policy
   *  @param twitterUrl - url of twitter feed
   *  @param facebookUrl - url of facebook feed
   *  @return - the data from the api
  */
  char * POSTMultipack( char *multipack_id, char *group_id, char *domainName, char *multipackName, char *less, char *country, char *menuTop, char *menuBottom, char *language, char *menuFooter, char *searchNumberResults, char *searchTitle, char *searchDescription, char *searchTitleNoWhere, char *searchDescriptionNoWhere, char *searchIntroHeader, char *searchIntroText, char *searchShowAll, char *searchUnitOfDistance, char *cookiePolicyShow, char *cookiePolicyUrl, char *twitterUrl, char *facebookUrl) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"multipackName=");
    p = curl_easy_escape(curl,multipackName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"less=");
    p = curl_easy_escape(curl,less,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"menuTop=");
    p = curl_easy_escape(curl,menuTop,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"menuBottom=");
    p = curl_easy_escape(curl,menuBottom,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"menuFooter=");
    p = curl_easy_escape(curl,menuFooter,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchNumberResults=");
    p = curl_easy_escape(curl,searchNumberResults,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchTitle=");
    p = curl_easy_escape(curl,searchTitle,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchDescription=");
    p = curl_easy_escape(curl,searchDescription,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchTitleNoWhere=");
    p = curl_easy_escape(curl,searchTitleNoWhere,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchDescriptionNoWhere=");
    p = curl_easy_escape(curl,searchDescriptionNoWhere,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchIntroHeader=");
    p = curl_easy_escape(curl,searchIntroHeader,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchIntroText=");
    p = curl_easy_escape(curl,searchIntroText,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchShowAll=");
    p = curl_easy_escape(curl,searchShowAll,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"searchUnitOfDistance=");
    p = curl_easy_escape(curl,searchUnitOfDistance,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"cookiePolicyShow=");
    p = curl_easy_escape(curl,cookiePolicyShow,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"cookiePolicyUrl=");
    p = curl_easy_escape(curl,cookiePolicyUrl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"twitterUrl=");
    p = curl_easy_escape(curl,twitterUrl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"facebookUrl=");
    p = curl_easy_escape(curl,facebookUrl,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/multipack",params);
  }


  /**
   * Get a multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETMultipack( char *multipack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/multipack",params);
  }


  /**
   * Add an admin theme to a multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTMultipackAdminCSS( char *multipack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/multipack/adminCSS",params);
  }


  /**
   * Add a Admin logo to a Multipack domain
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTMultipackAdminLogo( char *multipack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/multipack/adminLogo",params);
  }


  /**
   * Get a multipack using a domain name
   *
   *  @param domainName - the domain name to search for
   *  @return - the data from the api
  */
  char * GETMultipackBy_domain_name( char *domainName) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/multipack/by_domain_name",params);
  }


  /**
   * duplicates a given multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @param domainName - the domain name to serve this multipack site on (no leading http:// or anything please)
   *  @param group_id - the group to use for search
   *  @return - the data from the api
  */
  char * GETMultipackClone( char *multipack_id, char *domainName, char *group_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domainName=");
    p = curl_easy_escape(curl,domainName,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/multipack/clone",params);
  }


  /**
   * returns the LESS theme from a multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETMultipackLess( char *multipack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/multipack/less",params);
  }


  /**
   * Add a logo to a multipack domain
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTMultipackLogo( char *multipack_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/multipack/logo",params);
  }


  /**
   * Add a map pin to a multipack domain
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @param mapPinOffsetX
   *  @param mapPinOffsetY
   *  @return - the data from the api
  */
  char * POSTMultipackMap_pin( char *multipack_id, char *filedata, char *mapPinOffsetX, char *mapPinOffsetY) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapPinOffsetX=");
    p = curl_easy_escape(curl,mapPinOffsetX,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mapPinOffsetY=");
    p = curl_easy_escape(curl,mapPinOffsetY,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/multipack/map_pin",params);
  }


  /**
   * Fetch an ops_log
   *
   *  @param ops_log_id
   *  @return - the data from the api
  */
  char * GETOps_log( char *ops_log_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"ops_log_id=");
    p = curl_easy_escape(curl,ops_log_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ops_log",params);
  }


  /**
   * Create an ops_log
   *
   *  @param success
   *  @param type
   *  @param action
   *  @param data
   *  @param slack_channel
   *  @return - the data from the api
  */
  char * POSTOps_log( char *success, char *type, char *action, char *data, char *slack_channel) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"success=");
    p = curl_easy_escape(curl,success,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action=");
    p = curl_easy_escape(curl,action,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"slack_channel=");
    p = curl_easy_escape(curl,slack_channel,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/ops_log",params);
  }


  /**
   * Run PTB for a given ingest job ID.
   *
   *  @param ingest_job_id - The ingest job ID
   *  @param email - When all entity IDs are pushed to the PTB queue, an email containing debug info will be sent.
   *  @return - the data from the api
  */
  char * POSTPaintBy_ingest_job_id( char *ingest_job_id, char *email) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"ingest_job_id=");
    p = curl_easy_escape(curl,ingest_job_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/paint/by_ingest_job_id",params);
  }


  /**
   * With a known entity id syndication of data back to a partner is enabled
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateActivate( char *entity_id, char *publisher_id, char *expiry_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/partnersyndicate/activate",params);
  }


  /**
   * Call CK syndication instruction and call cancel endpoint for partner/supplier_id
   *
   *  @param supplierid
   *  @param vendor
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateCancel( char *supplierid, char *vendor) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"supplierid=");
    p = curl_easy_escape(curl,supplierid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"vendor=");
    p = curl_easy_escape(curl,vendor,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/partnersyndicate/cancel",params);
  }


  /**
   * This will call into CK in order to create the entity on the third party system.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateCreate( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/partnersyndicate/create",params);
  }


  /**
   * If this call fails CK is nudged for a human intervention for the future (so the call is NOT passive)
   *
   *  @param vendor_cat_id
   *  @param vendor_cat_string
   *  @param vendor
   *  @return - the data from the api
  */
  char * GETPartnersyndicateRequestcat( char *vendor_cat_id, char *vendor_cat_string, char *vendor) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"vendor_cat_id=");
    p = curl_easy_escape(curl,vendor_cat_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"vendor_cat_string=");
    p = curl_easy_escape(curl,vendor_cat_string,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"vendor=");
    p = curl_easy_escape(curl,vendor,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/partnersyndicate/requestcat",params);
  }


  /**
   * This will do nothing if the entity does not have a current partnersyndicate block. Syndication is invoked automatically when entities are saved, so this endpoint is designed for checking the status of syndication.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateUpdateToCk( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/partnersyndicate/updateToCk",params);
  }


  /**
   * When a plugin is added to the system it must be added to the service
   *
   *  @param id
   *  @param slug
   *  @param owner
   *  @param scope
   *  @param status
   *  @param params
   *  @return - the data from the api
  */
  char * POSTPlugin( char *id, char *slug, char *owner, char *scope, char *status, char *params) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"slug=");
    p = curl_easy_escape(curl,slug,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"owner=");
    p = curl_easy_escape(curl,owner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"scope=");
    p = curl_easy_escape(curl,scope,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"status=");
    p = curl_easy_escape(curl,status,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"params=");
    p = curl_easy_escape(curl,params,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/plugin",params);
  }


  /**
   * Get plugin data
   *
   *  @param id
   *  @return - the data from the api
  */
  char * GETPlugin( char *id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/plugin",params);
  }


  /**
   * With a known entity id, a plugin is enabled
   *
   *  @param entity_id
   *  @param plugin
   *  @param inapp_name
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTPluginActivate( char *entity_id, char *plugin, char *inapp_name, char *expiry_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"plugin=");
    p = curl_easy_escape(curl,plugin,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inapp_name=");
    p = curl_easy_escape(curl,inapp_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/plugin/activate",params);
  }


  /**
   * With a known entity id, a plugin is cancelled
   *
   *  @param entity_id
   *  @param plugin
   *  @param inapp_name
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTPluginCancel( char *entity_id, char *plugin, char *inapp_name, char *expiry_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"plugin=");
    p = curl_easy_escape(curl,plugin,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inapp_name=");
    p = curl_easy_escape(curl,inapp_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/plugin/cancel",params);
  }


  /**
   * Arbitrary big data
   *
   *  @param pluginid
   *  @param name
   *  @param filter1
   *  @param filter2
   *  @param order
   *  @param fields - a json string with up to 20 fields indexed 'field1' thru 'field20'
   *  @return - the data from the api
  */
  char * GETPluginDatarow( char *pluginid, char *name, char *filter1, char *filter2, char *order, char *fields) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"pluginid=");
    p = curl_easy_escape(curl,pluginid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filter1=");
    p = curl_easy_escape(curl,filter1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filter2=");
    p = curl_easy_escape(curl,filter2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"order=");
    p = curl_easy_escape(curl,order,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"fields=");
    p = curl_easy_escape(curl,fields,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/plugin/datarow",params);
  }


  /**
   * Arbitrary big data
   *
   *  @param rowdataid
   *  @param pluginid
   *  @param name
   *  @param filter1
   *  @param filter2
   *  @param fields - a json string with up to 20 fields indexed 'field1' thru 'field20'
   *  @return - the data from the api
  */
  char * POSTPluginDatarow( char *rowdataid, char *pluginid, char *name, char *filter1, char *filter2, char *fields) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"rowdataid=");
    p = curl_easy_escape(curl,rowdataid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"pluginid=");
    p = curl_easy_escape(curl,pluginid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filter1=");
    p = curl_easy_escape(curl,filter1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filter2=");
    p = curl_easy_escape(curl,filter2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"fields=");
    p = curl_easy_escape(curl,fields,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/plugin/datarow",params);
  }


  /**
   * With a known entity id, a plugin is enabled
   *
   *  @param pluginid
   *  @param userid
   *  @param entity_id
   *  @param storekey
   *  @param storeval
   *  @return - the data from the api
  */
  char * POSTPluginVar( char *pluginid, char *userid, char *entity_id, char *storekey, char *storeval) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"pluginid=");
    p = curl_easy_escape(curl,pluginid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"userid=");
    p = curl_easy_escape(curl,userid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"storekey=");
    p = curl_easy_escape(curl,storekey,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"storeval=");
    p = curl_easy_escape(curl,storeval,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/plugin/var",params);
  }


  /**
   * Get variables related to a particular entity
   *
   *  @param entityid
   *  @return - the data from the api
  */
  char * GETPluginVarsByEntityId( char *entityid) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entityid=");
    p = curl_easy_escape(curl,entityid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/plugin/vars/byEntityId",params);
  }


  /**
   * Get info on all plugins
   *
   *  @return - the data from the api
  */
  char * GETPlugins() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/plugins",params);
  }


  /**
   * Allows a private object to be removed
   *
   *  @param private_object_id - The id of the private object to remove
   *  @return - the data from the api
  */
  char * DELETEPrivate_object( char *private_object_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"private_object_id=");
    p = curl_easy_escape(curl,private_object_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/private_object",params);
  }


  /**
   * With a known entity id, a private object can be added.
   *
   *  @param entity_id - The entity to associate the private object with
   *  @param data - The data to store
   *  @return - the data from the api
  */
  char * PUTPrivate_object( char *entity_id, char *data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/private_object",params);
  }


  /**
   * Allows a private object to be returned based on the entity_id and masheryid
   *
   *  @param entity_id - The entity associated with the private object
   *  @return - the data from the api
  */
  char * GETPrivate_objectAll( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/private_object/all",params);
  }


  /**
   * Update/Add a product
   *
   *  @param product_id - The ID of the product
   *  @param shortname - Desc
   *  @param name - The name of the product
   *  @param strapline - The description of the product
   *  @param alternate_title - The alternate title of the product
   *  @param fpzones - Hints for flatpack display (set a single hint 'void' to have this ignored)
   *  @param paygateid - The product id in the payment gateway (required for Stripe)
   *  @param price - The price of the product
   *  @param tax_rate - The tax markup for this product
   *  @param currency - The currency in which the price is in
   *  @param active - is this an active product
   *  @param billing_period
   *  @param title - Title of the product
   *  @param intro_paragraph - introduction paragraph
   *  @param bullets - pipe separated product features
   *  @param outro_paragraph - closing paragraph
   *  @param product_description_html - Overriding product description html blob
   *  @param thankyou_html - overriding thank you paragraph html
   *  @param thanks_paragraph - thank you paragraph
   *  @param video_url - video url
   *  @return - the data from the api
  */
  char * POSTProduct( char *product_id, char *shortname, char *name, char *strapline, char *alternate_title, char *fpzones, char *paygateid, char *price, char *tax_rate, char *currency, char *active, char *billing_period, char *title, char *intro_paragraph, char *bullets, char *outro_paragraph, char *product_description_html, char *thankyou_html, char *thanks_paragraph, char *video_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"shortname=");
    p = curl_easy_escape(curl,shortname,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"strapline=");
    p = curl_easy_escape(curl,strapline,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"alternate_title=");
    p = curl_easy_escape(curl,alternate_title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"fpzones=");
    p = curl_easy_escape(curl,fpzones,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"paygateid=");
    p = curl_easy_escape(curl,paygateid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"price=");
    p = curl_easy_escape(curl,price,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tax_rate=");
    p = curl_easy_escape(curl,tax_rate,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency=");
    p = curl_easy_escape(curl,currency,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"billing_period=");
    p = curl_easy_escape(curl,billing_period,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"title=");
    p = curl_easy_escape(curl,title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"intro_paragraph=");
    p = curl_easy_escape(curl,intro_paragraph,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"bullets=");
    p = curl_easy_escape(curl,bullets,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"outro_paragraph=");
    p = curl_easy_escape(curl,outro_paragraph,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"product_description_html=");
    p = curl_easy_escape(curl,product_description_html,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"thankyou_html=");
    p = curl_easy_escape(curl,thankyou_html,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"thanks_paragraph=");
    p = curl_easy_escape(curl,thanks_paragraph,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"video_url=");
    p = curl_easy_escape(curl,video_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product",params);
  }


  /**
   * Returns the product information given a valid product_id
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * GETProduct( char *product_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/product",params);
  }


  /**
   * Uploads logo image to product
   *
   *  @param product_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTProductImageLogo( char *product_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/image/logo",params);
  }


  /**
   * Delete the logo image within a specific product
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * DELETEProductImageLogo( char *product_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/product/image/logo",params);
  }


  /**
   * Delete the main image within a specific product
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * DELETEProductImageMain( char *product_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/product/image/main",params);
  }


  /**
   * Adds partblahnersyndicate provisioning object to a product
   *
   *  @param product_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTProductImageMain( char *product_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/image/main",params);
  }


  /**
   * Delete the small image within a specific product
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * DELETEProductImageSmall( char *product_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/product/image/small",params);
  }


  /**
   * Uploads small image to product
   *
   *  @param product_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTProductImageSmall( char *product_id, char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/image/small",params);
  }


  /**
   * Removes a provisioning object from product
   *
   *  @param product_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEProductProvisioning( char *product_id, char *gen_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/product/provisioning",params);
  }


  /**
   * Adds advertising provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @param max_tags
   *  @param max_locations
   *  @return - the data from the api
  */
  char * POSTProductProvisioningAdvert( char *product_id, char *publisher_id, char *max_tags, char *max_locations) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_tags=");
    p = curl_easy_escape(curl,max_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_locations=");
    p = curl_easy_escape(curl,max_locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/advert",params);
  }


  /**
   * Adds claim provisioning object to a product
   *
   *  @param product_id
   *  @param package
   *  @return - the data from the api
  */
  char * POSTProductProvisioningClaim( char *product_id, char *package) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"package=");
    p = curl_easy_escape(curl,package,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/claim",params);
  }


  /**
   * Adds maxclaims provisioning object to a product
   *
   *  @param product_id
   *  @param country
   *  @param number
   *  @return - the data from the api
  */
  char * POSTProductProvisioningMaxclaims( char *product_id, char *country, char *number) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"number=");
    p = curl_easy_escape(curl,number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/maxclaims",params);
  }


  /**
   * Adds partnersyndicate provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTProductProvisioningPartnersyndicate( char *product_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/partnersyndicate",params);
  }


  /**
   * Adds plugin provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTProductProvisioningPlugin( char *product_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/plugin",params);
  }


  /**
   * Adds SCheduleSMS provisioning object to a product
   *
   *  @param product_id
   *  @param package
   *  @return - the data from the api
  */
  char * POSTProductProvisioningSchedulesms( char *product_id, char *package) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"package=");
    p = curl_easy_escape(curl,package,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/schedulesms",params);
  }


  /**
   * Adds syndication provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTProductProvisioningSyndication( char *product_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/product/provisioning/syndication",params);
  }


  /**
   * Perform the whole PTB process on the supplied entity
   *
   *  @param entity_id
   *  @param destructive
   *  @return - the data from the api
  */
  char * GETPtbAll( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ptb/all",params);
  }


  /**
   * Report on what happened to specific entity_id
   *
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param entity_id - the entity to research
   *  @return - the data from the api
  */
  char * GETPtbLog( char *year, char *month, char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"year=");
    p = curl_easy_escape(curl,year,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"month=");
    p = curl_easy_escape(curl,month,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ptb/log",params);
  }


  /**
   * Process an entity with a specific PTB module
   *
   *  @param entity_id
   *  @param module
   *  @param destructive
   *  @return - the data from the api
  */
  char * GETPtbModule( char *entity_id, char *module, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"module=");
    p = curl_easy_escape(curl,module,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ptb/module",params);
  }


  /**
   * Report on the run-rate of the Paint the Bridge System
   *
   *  @param country - the country to get the runrate for
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param day - the day to examine
   *  @return - the data from the api
  */
  char * GETPtbRunrate( char *country, char *year, char *month, char *day) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"year=");
    p = curl_easy_escape(curl,year,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"month=");
    p = curl_easy_escape(curl,month,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"day=");
    p = curl_easy_escape(curl,day,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ptb/runrate",params);
  }


  /**
   * Report on the value being added by Paint The Bridge
   *
   *  @param country - the country to get the runrate for
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param day - the day to examine
   *  @return - the data from the api
  */
  char * GETPtbValueadded( char *country, char *year, char *month, char *day) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"year=");
    p = curl_easy_escape(curl,year,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"month=");
    p = curl_easy_escape(curl,month,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"day=");
    p = curl_easy_escape(curl,day,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/ptb/valueadded",params);
  }


  /**
   * Returns publisher that matches a given publisher id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * GETPublisher( char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/publisher",params);
  }


  /**
   * Update/Add a publisher
   *
   *  @param publisher_id
   *  @param country
   *  @param name
   *  @param description
   *  @param active
   *  @param url_patterns
   *  @param premium_adverts_platinum
   *  @param premium_adverts_gold
   *  @return - the data from the api
  */
  char * POSTPublisher( char *publisher_id, char *country, char *name, char *description, char *active, char *url_patterns, char *premium_adverts_platinum, char *premium_adverts_gold) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"url_patterns=");
    p = curl_easy_escape(curl,url_patterns,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"premium_adverts_platinum=");
    p = curl_easy_escape(curl,premium_adverts_platinum,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"premium_adverts_gold=");
    p = curl_easy_escape(curl,premium_adverts_gold,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/publisher",params);
  }


  /**
   * Delete a publisher with a specified publisher_id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * DELETEPublisher( char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/publisher",params);
  }


  /**
   * Returns publisher that matches a given publisher id
   *
   *  @param country
   *  @return - the data from the api
  */
  char * GETPublisherByCountry( char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/publisher/byCountry",params);
  }


  /**
   * Returns publishers that are available for a given entity_id.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETPublisherByEntityId( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/publisher/byEntityId",params);
  }


  /**
   * Returns a publisher that has the specified masheryid
   *
   *  @param publisher_masheryid
   *  @return - the data from the api
  */
  char * GETPublisherBy_masheryid( char *publisher_masheryid) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"publisher_masheryid=");
    p = curl_easy_escape(curl,publisher_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/publisher/by_masheryid",params);
  }


  /**
   * Retrieve queue items.
   *
   *  @param limit
   *  @param queue_name
   *  @return - the data from the api
  */
  char * GETQueue( char *limit, char *queue_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"queue_name=");
    p = curl_easy_escape(curl,queue_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/queue",params);
  }


  /**
   * Create a queue item
   *
   *  @param queue_name
   *  @param data
   *  @return - the data from the api
  */
  char * PUTQueue( char *queue_name, char *data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_name=");
    p = curl_easy_escape(curl,queue_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/queue",params);
  }


  /**
   * With a known queue id, a queue item can be removed.
   *
   *  @param queue_id
   *  @return - the data from the api
  */
  char * DELETEQueue( char *queue_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_id=");
    p = curl_easy_escape(curl,queue_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/queue",params);
  }


  /**
   * Find a queue item by its cloudant id
   *
   *  @param queue_id
   *  @return - the data from the api
  */
  char * GETQueueBy_id( char *queue_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_id=");
    p = curl_easy_escape(curl,queue_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/queue/by_id",params);
  }


  /**
   * Add an error to a queue item
   *
   *  @param queue_id
   *  @param error
   *  @return - the data from the api
  */
  char * POSTQueueError( char *queue_id, char *error) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_id=");
    p = curl_easy_escape(curl,queue_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"error=");
    p = curl_easy_escape(curl,error,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/queue/error",params);
  }


  /**
   * Find a queue item by its type and id
   *
   *  @param type
   *  @param id
   *  @return - the data from the api
  */
  char * GETQueueSearch( char *type, char *id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/queue/search",params);
  }


  /**
   * Unlock queue items.
   *
   *  @param queue_name
   *  @param seconds
   *  @return - the data from the api
  */
  char * POSTQueueUnlock( char *queue_name, char *seconds) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_name=");
    p = curl_easy_escape(curl,queue_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"seconds=");
    p = curl_easy_escape(curl,seconds,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/queue/unlock",params);
  }


  /**
   * Create an SQS queue item
   *
   *  @param queue_name
   *  @param data
   *  @return - the data from the api
  */
  char * PUTQueue_sqs( char *queue_name, char *data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_name=");
    p = curl_easy_escape(curl,queue_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/queue_sqs",params);
  }


  /**
   * Get the attributes of an SQS queue
   *
   *  @param queue_name
   *  @return - the data from the api
  */
  char * GETQueue_sqsAttributes( char *queue_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"queue_name=");
    p = curl_easy_escape(curl,queue_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/queue_sqs/attributes",params);
  }


  /**
   * Returns reseller that matches a given reseller id
   *
   *  @param reseller_id
   *  @return - the data from the api
  */
  char * GETReseller( char *reseller_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"reseller_id=");
    p = curl_easy_escape(curl,reseller_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/reseller",params);
  }


  /**
   * Update/Add a reseller
   *
   *  @param reseller_id
   *  @param country
   *  @param name
   *  @param description
   *  @param active
   *  @param products
   *  @param master_user_id
   *  @param canViewEmployee
   *  @return - the data from the api
  */
  char * POSTReseller( char *reseller_id, char *country, char *name, char *description, char *active, char *products, char *master_user_id, char *canViewEmployee) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"reseller_id=");
    p = curl_easy_escape(curl,reseller_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"products=");
    p = curl_easy_escape(curl,products,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"master_user_id=");
    p = curl_easy_escape(curl,master_user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"canViewEmployee=");
    p = curl_easy_escape(curl,canViewEmployee,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/reseller",params);
  }


  /**
   * Return a sales log by id
   *
   *  @param sales_log_id - The sales log id to pull
   *  @return - the data from the api
  */
  char * GETSales_log( char *sales_log_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"sales_log_id=");
    p = curl_easy_escape(curl,sales_log_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/sales_log",params);
  }


  /**
   * Return a sales log by id
   *
   *  @param from_date
   *  @param country
   *  @param action_type
   *  @return - the data from the api
  */
  char * GETSales_logBy_countryBy_date( char *from_date, char *country, char *action_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/sales_log/by_country/by_date",params);
  }


  /**
   * Return a sales log by date range, filtered by masheryid if it is given
   *
   *  @param from_date
   *  @param to_date
   *  @param group
   *  @param limit - Applicable only when group=false
   *  @param skip - Applicable only when group=false
   *  @return - the data from the api
  */
  char * GETSales_logBy_date( char *from_date, char *to_date, char *group, char *limit, char *skip) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from_date=");
    p = curl_easy_escape(curl,from_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to_date=");
    p = curl_easy_escape(curl,to_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group=");
    p = curl_easy_escape(curl,group,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip=");
    p = curl_easy_escape(curl,skip,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/sales_log/by_date",params);
  }


  /**
   * Log a sale
   *
   *  @param entity_id - The entity the sale was made against
   *  @param country - The country code the sales log orginated
   *  @param action_type - The type of action we are performing
   *  @param ad_type - The type of advertisements
   *  @param publisher_id - The publisher id that has made the sale
   *  @param mashery_id - The mashery id
   *  @param reseller_ref - The reference of the sale made by the seller
   *  @param reseller_agent_id - The id of the agent selling the product
   *  @param max_tags - The number of tags available to the entity
   *  @param max_locations - The number of locations available to the entity
   *  @param extra_tags - The extra number of tags
   *  @param extra_locations - The extra number of locations
   *  @param expiry_date - The date the product expires
   *  @return - the data from the api
  */
  char * POSTSales_logEntity( char *entity_id, char *country, char *action_type, char *ad_type, char *publisher_id, char *mashery_id, char *reseller_ref, char *reseller_agent_id, char *max_tags, char *max_locations, char *extra_tags, char *extra_locations, char *expiry_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"ad_type=");
    p = curl_easy_escape(curl,ad_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"mashery_id=");
    p = curl_easy_escape(curl,mashery_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_ref=");
    p = curl_easy_escape(curl,reseller_ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_agent_id=");
    p = curl_easy_escape(curl,reseller_agent_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_tags=");
    p = curl_easy_escape(curl,max_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_locations=");
    p = curl_easy_escape(curl,max_locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"extra_tags=");
    p = curl_easy_escape(curl,extra_tags,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"extra_locations=");
    p = curl_easy_escape(curl,extra_locations,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/sales_log/entity",params);
  }


  /**
   * Add a Sales Log document for a syndication action
   *
   *  @param action_type
   *  @param syndication_type
   *  @param publisher_id
   *  @param expiry_date
   *  @param entity_id
   *  @param group_id
   *  @param seed_masheryid
   *  @param supplier_masheryid
   *  @param country
   *  @param reseller_masheryid
   *  @return - the data from the api
  */
  char * POSTSales_logSyndication( char *action_type, char *syndication_type, char *publisher_id, char *expiry_date, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *reseller_masheryid) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"syndication_type=");
    p = curl_easy_escape(curl,syndication_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"seed_masheryid=");
    p = curl_easy_escape(curl,seed_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_masheryid=");
    p = curl_easy_escape(curl,supplier_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_masheryid=");
    p = curl_easy_escape(curl,reseller_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/sales_log/syndication",params);
  }


  /**
   * Converts an Entity into a submission at the Scoot Partner API
   *
   *  @param entity_id - The entity to parse
   *  @param reseller - The reseller Mashery ID, it also determines which Scoot API key to use
   *  @param scoot_id - If specified, the related Scoot listing will be updated.
   *  @param autofill_scoot_id - If scoot_id is not given, look for past successful syndication logs to fill in the Scoot ID
   *  @return - the data from the api
  */
  char * POSTScoot_priority( char *entity_id, char *reseller, char *scoot_id, char *autofill_scoot_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller=");
    p = curl_easy_escape(curl,reseller,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"scoot_id=");
    p = curl_easy_escape(curl,scoot_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"autofill_scoot_id=");
    p = curl_easy_escape(curl,autofill_scoot_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/scoot_priority",params);
  }


  /**
   * Make a url shorter
   *
   *  @param url - the url to shorten
   *  @param idOnly - Return just the Shortened URL ID
   *  @return - the data from the api
  */
  char * PUTShortenurl( char *url, char *idOnly) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"url=");
    p = curl_easy_escape(curl,url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"idOnly=");
    p = curl_easy_escape(curl,idOnly,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/shortenurl",params);
  }


  /**
   * get the long url from the db
   *
   *  @param id - the id to fetch from the db
   *  @return - the data from the api
  */
  char * GETShortenurl( char *id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/shortenurl",params);
  }


  /**
   * For insance, reporting a phone number as wrong
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param country - The country code from where the signal originated e.g. ie
   *  @param gen_id - The gen_id for the item being reported
   *  @param signal_type - The signal that is to be reported e.g. wrong
   *  @param data_type - The type of data being reported
   *  @param inactive_reason - The reason for making the entity inactive
   *  @param inactive_description - A description to accompany the inactive reasoning
   *  @param feedback - Some feedback from the person submitting the signal
   *  @return - the data from the api
  */
  char * POSTSignal( char *entity_id, char *country, char *gen_id, char *signal_type, char *data_type, char *inactive_reason, char *inactive_description, char *feedback) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"gen_id=");
    p = curl_easy_escape(curl,gen_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"signal_type=");
    p = curl_easy_escape(curl,signal_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data_type=");
    p = curl_easy_escape(curl,data_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inactive_reason=");
    p = curl_easy_escape(curl,inactive_reason,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"inactive_description=");
    p = curl_easy_escape(curl,inactive_description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"feedback=");
    p = curl_easy_escape(curl,feedback,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/signal",params);
  }


  /**
   * With a given country and entity id suffix, this endpoint will return a list of entity IDs and their last modified dates for sitemap generation.
   *
   *  @param country - Target country code.
   *  @param id_suffix - Target entity Id suffix (4 digits).
   *  @param skip
   *  @param limit
   *  @return - the data from the api
  */
  char * GETSitemapEntity( char *country, char *id_suffix, char *skip, char *limit) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"id_suffix=");
    p = curl_easy_escape(curl,id_suffix,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"skip=");
    p = curl_easy_escape(curl,skip,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"limit=");
    p = curl_easy_escape(curl,limit,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/sitemap/entity",params);
  }


  /**
   * With a given country, this endpoint will return a list of entity ID suffixes which have records.
   *
   *  @param country - Target country code.
   *  @return - the data from the api
  */
  char * GETSitemapEntitySummary( char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/sitemap/entity/summary",params);
  }


  /**
   * Get a spider document
   *
   *  @param spider_id
   *  @return - the data from the api
  */
  char * GETSpider( char *spider_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"spider_id=");
    p = curl_easy_escape(curl,spider_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/spider",params);
  }


  /**
   * Get the number of times an entity has been served out as an advert or on serps/bdp pages
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @param month - The month to report on
   *  @return - the data from the api
  */
  char * GETStatsEntityBy_date( char *entity_id, char *year, char *month) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"year=");
    p = curl_easy_escape(curl,year,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"month=");
    p = curl_easy_escape(curl,month,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/stats/entity/by_date",params);
  }


  /**
   * Get the stats on an entity in a given year
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @return - the data from the api
  */
  char * GETStatsEntityBy_year( char *entity_id, char *year) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"year=");
    p = curl_easy_escape(curl,year,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/stats/entity/by_year",params);
  }


  /**
   * Confirms that the API is active, and returns the current version number
   *
   *  @return - the data from the api
  */
  char * GETStatus() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/status",params);
  }


  /**
   * get a Syndication
   *
   *  @param syndication_id
   *  @return - the data from the api
  */
  char * GETSyndication( char *syndication_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_id=");
    p = curl_easy_escape(curl,syndication_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/syndication",params);
  }


  /**
   * get a Syndication by entity_id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETSyndicationBy_entity_id( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/syndication/by_entity_id",params);
  }


  /**
   * Get a Syndication by Reseller (Mashery ID) and optional entity ID
   *
   *  @param reseller_masheryid
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETSyndicationBy_reseller( char *reseller_masheryid, char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"reseller_masheryid=");
    p = curl_easy_escape(curl,reseller_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/syndication/by_reseller",params);
  }


  /**
   * Cancel a syndication
   *
   *  @param syndication_id
   *  @return - the data from the api
  */
  char * POSTSyndicationCancel( char *syndication_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_id=");
    p = curl_easy_escape(curl,syndication_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/syndication/cancel",params);
  }


  /**
   * Add a Syndicate
   *
   *  @param syndication_type
   *  @param publisher_id
   *  @param expiry_date
   *  @param entity_id
   *  @param group_id
   *  @param seed_masheryid
   *  @param supplier_masheryid
   *  @param country
   *  @param data_filter
   *  @return - the data from the api
  */
  char * POSTSyndicationCreate( char *syndication_type, char *publisher_id, char *expiry_date, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *data_filter) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_type=");
    p = curl_easy_escape(curl,syndication_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"seed_masheryid=");
    p = curl_easy_escape(curl,seed_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_masheryid=");
    p = curl_easy_escape(curl,supplier_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data_filter=");
    p = curl_easy_escape(curl,data_filter,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/syndication/create",params);
  }


  /**
   * Renew a Syndicate
   *
   *  @param syndication_type
   *  @param publisher_id
   *  @param entity_id
   *  @param group_id
   *  @param seed_masheryid
   *  @param supplier_masheryid
   *  @param country
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTSyndicationRenew( char *syndication_type, char *publisher_id, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *expiry_date) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_type=");
    p = curl_easy_escape(curl,syndication_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"seed_masheryid=");
    p = curl_easy_escape(curl,seed_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_masheryid=");
    p = curl_easy_escape(curl,supplier_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expiry_date=");
    p = curl_easy_escape(curl,expiry_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/syndication/renew",params);
  }


  /**
   * When we get a syndication update make a record of it
   *
   *  @param entity_id - The entity to pull
   *  @param publisher_id - The publisher this log entry refers to
   *  @param action - The log type
   *  @param success - If the syndication was successful
   *  @param message - An optional message e.g. submitted to the syndication partner
   *  @param syndicated_id - The entity as known to the publisher
   *  @param reseller_id - The optional reseller id used in the syndications
   *  @return - the data from the api
  */
  char * POSTSyndication_log( char *entity_id, char *publisher_id, char *action, char *success, char *message, char *syndicated_id, char *reseller_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action=");
    p = curl_easy_escape(curl,action,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"success=");
    p = curl_easy_escape(curl,success,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"message=");
    p = curl_easy_escape(curl,message,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"syndicated_id=");
    p = curl_easy_escape(curl,syndicated_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_id=");
    p = curl_easy_escape(curl,reseller_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/syndication_log",params);
  }


  /**
   * Get all syndication log entries for a given entity id
   *
   *  @param entity_id
   *  @param page
   *  @param per_page
   *  @return - the data from the api
  */
  char * GETSyndication_logBy_entity_id( char *entity_id, char *page, char *per_page) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"page=");
    p = curl_easy_escape(curl,page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"per_page=");
    p = curl_easy_escape(curl,per_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/syndication_log/by_entity_id",params);
  }


  /**
   * Get the latest syndication log feedback entry for a given entity id and publisher
   *
   *  @param entity_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * GETSyndication_logLast_syndicated_id( char *entity_id, char *publisher_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/syndication_log/last_syndicated_id",params);
  }


  /**
   * Creates a new Syndication Submission
   *
   *  @param syndication_type
   *  @param entity_id
   *  @param publisher_id
   *  @param submission_id
   *  @return - the data from the api
  */
  char * PUTSyndication_submission( char *syndication_type, char *entity_id, char *publisher_id, char *submission_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_type=");
    p = curl_easy_escape(curl,syndication_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_id=");
    p = curl_easy_escape(curl,publisher_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"submission_id=");
    p = curl_easy_escape(curl,submission_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/syndication_submission",params);
  }


  /**
   * Returns a Syndication Submission
   *
   *  @param syndication_submission_id
   *  @return - the data from the api
  */
  char * GETSyndication_submission( char *syndication_submission_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_submission_id=");
    p = curl_easy_escape(curl,syndication_submission_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/syndication_submission",params);
  }


  /**
   * Set active to false for a Syndication Submission
   *
   *  @param syndication_submission_id
   *  @return - the data from the api
  */
  char * POSTSyndication_submissionDeactivate( char *syndication_submission_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_submission_id=");
    p = curl_easy_escape(curl,syndication_submission_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/syndication_submission/deactivate",params);
  }


  /**
   * Set the processed date to now for a Syndication Submission
   *
   *  @param syndication_submission_id
   *  @return - the data from the api
  */
  char * POSTSyndication_submissionProcessed( char *syndication_submission_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_submission_id=");
    p = curl_easy_escape(curl,syndication_submission_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/syndication_submission/processed",params);
  }


  /**
   * Provides a tokenised URL to redirect a user so they can add an entity to Central Index
   *
   *  @param language - The language to use to render the add path e.g. en
   *  @param business_name - The name of the business (to be presented as a default) e.g. The Dog and Duck
   *  @param business_phone - The phone number of the business (to be presented as a default) e.g. 20 8480-2777
   *  @param business_postcode - The postcode of the business (to be presented as a default) e.g. EC1 1AA
   *  @param portal_name - The name of the website that data is to be added on e.g. YourLocal
   *  @param supplier_id - The supplier id e.g. 1234xxx889
   *  @param partner - syndication partner (eg 192)
   *  @param country - The country of the entity to be added e.g. gb
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenAdd( char *language, char *business_name, char *business_phone, char *business_postcode, char *portal_name, char *supplier_id, char *partner, char *country, char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"business_name=");
    p = curl_easy_escape(curl,business_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"business_phone=");
    p = curl_easy_escape(curl,business_phone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"business_postcode=");
    p = curl_easy_escape(curl,business_postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_id=");
    p = curl_easy_escape(curl,supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"partner=");
    p = curl_easy_escape(curl,partner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/add",params);
  }


  /**
   * Provides a tokenised URL to redirect a user to claim an entity on Central Index
   *
   *  @param entity_id - Entity ID to be claimed e.g. 380348266819584
   *  @param supplier_id - Supplier ID to be added (along with masheryid) e.g. 380348266819584
   *  @param language - The language to use to render the claim path e.g. en
   *  @param portal_name - The name of the website that entity is being claimed on e.g. YourLocal
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param admin_host - The admin host to refer back to - will only be respected if whitelisted in configuration
   *  @return - the data from the api
  */
  char * GETTokenClaim( char *entity_id, char *supplier_id, char *language, char *portal_name, char *flatpack_id, char *admin_host) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"supplier_id=");
    p = curl_easy_escape(curl,supplier_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"admin_host=");
    p = curl_easy_escape(curl,admin_host,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/claim",params);
  }


  /**
   * Fetch token for the contact us method
   *
   *  @param portal_name - The portal name
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param language - en, es, etc...
   *  @param referring_url - the url where the request came from
   *  @return - the data from the api
  */
  char * GETTokenContact_us( char *portal_name, char *flatpack_id, char *language, char *referring_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referring_url=");
    p = curl_easy_escape(curl,referring_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/contact_us",params);
  }


  /**
   * Allows us to identify the user, entity and element from an encoded endpoint URL's token
   *
   *  @param token
   *  @return - the data from the api
  */
  char * GETTokenDecode( char *token) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"token=");
    p = curl_easy_escape(curl,token,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/decode",params);
  }


  /**
   * Fetch token for edit path
   *
   *  @param entity_id - The id of the entity being upgraded
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param edit_page - the page in the edit path that the user should land on
   *  @return - the data from the api
  */
  char * GETTokenEdit( char *entity_id, char *language, char *flatpack_id, char *edit_page) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"edit_page=");
    p = curl_easy_escape(curl,edit_page,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/edit",params);
  }


  /**
   * Fetch token for some admin page.
   *
   *  @param portal_name - The name of the application that has initiated the login process, example: 'Your Local'
   *  @param code - Secret string which will be validated by the target page.
   *  @param expireAt - When this token expires in UNIX timestamp. The target page should validate this.
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param multipack_id - The id of the multipack site where the request originated
   *  @param data - Optional extra data to be passed to the targeted page.
   *  @return - the data from the api
  */
  char * GETTokenEncode( char *portal_name, char *code, char *expireAt, char *language, char *flatpack_id, char *multipack_id, char *data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"code=");
    p = curl_easy_escape(curl,code,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"expireAt=");
    p = curl_easy_escape(curl,expireAt,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/encode",params);
  }


  /**
   * Fetch token for login path
   *
   *  @param portal_name - The name of the application that has initiated the login process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param multipack_id - The id of the multipack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenLogin( char *portal_name, char *language, char *flatpack_id, char *multipack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"multipack_id=");
    p = curl_easy_escape(curl,multipack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/login",params);
  }


  /**
   * Get a tokenised url for an password reset
   *
   *  @param login_id - Login id
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param entity_id
   *  @param action
   *  @return - the data from the api
  */
  char * GETTokenLoginReset_password( char *login_id, char *flatpack_id, char *entity_id, char *action) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"login_id=");
    p = curl_easy_escape(curl,login_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action=");
    p = curl_easy_escape(curl,action,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/login/reset_password",params);
  }


  /**
   * Get a tokenised url for an email verification
   *
   *  @param email - Email address
   *  @param first_name - First name of the new user
   *  @param last_name - Last name of the new user
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param entity_id
   *  @param action
   *  @return - the data from the api
  */
  char * GETTokenLoginSet_password( char *email, char *first_name, char *last_name, char *flatpack_id, char *entity_id, char *action) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"first_name=");
    p = curl_easy_escape(curl,first_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"last_name=");
    p = curl_easy_escape(curl,last_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action=");
    p = curl_easy_escape(curl,action,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/login/set_password",params);
  }


  /**
   * Fetch token for messaging path
   *
   *  @param entity_id - The id of the entity being messaged
   *  @param portal_name - The name of the application that has initiated the email process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenMessage( char *entity_id, char *portal_name, char *language, char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/message",params);
  }


  /**
   * Fetch token for partnerclaim path
   *
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param partner - The partner (eg 192)
   *  @param partnerid - the supplier id from the partner site
   *  @param preclaimed - is this already claimed on the partner site (used for messaging)
   *  @return - the data from the api
  */
  char * GETTokenPartnerclaim( char *language, char *flatpack_id, char *partner, char *partnerid, char *preclaimed) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"partner=");
    p = curl_easy_escape(curl,partner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"partnerid=");
    p = curl_easy_escape(curl,partnerid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"preclaimed=");
    p = curl_easy_escape(curl,preclaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/partnerclaim",params);
  }


  /**
   * Fetch token for partnerclaim path (ie we start at a CI entity id rather than an external partner id)
   *
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param partner - The partner (eg 192)
   *  @param entityid - the CI entity id
   *  @param preclaimed - is this already claimed on the partner site (used for messaging)
   *  @return - the data from the api
  */
  char * GETTokenPartnerclaimInternal( char *language, char *flatpack_id, char *partner, char *entityid, char *preclaimed) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"partner=");
    p = curl_easy_escape(curl,partner,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entityid=");
    p = curl_easy_escape(curl,entityid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"preclaimed=");
    p = curl_easy_escape(curl,preclaimed,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/partnerclaim/internal",params);
  }


  /**
   * Fetch token for product path
   *
   *  @param entity_id - The id of the entity to add a product to
   *  @param product_id - The product id of the product
   *  @param language - The language for the app
   *  @param portal_name - The portal name
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param source - email, social media etc
   *  @param channel - 
   *  @param campaign - the campaign identifier
   *  @return - the data from the api
  */
  char * GETTokenProduct( char *entity_id, char *product_id, char *language, char *portal_name, char *flatpack_id, char *source, char *channel, char *campaign) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"product_id=");
    p = curl_easy_escape(curl,product_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"source=");
    p = curl_easy_escape(curl,source,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"channel=");
    p = curl_easy_escape(curl,channel,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"campaign=");
    p = curl_easy_escape(curl,campaign,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/product",params);
  }


  /**
   * Fetch token for product path
   *
   *  @param entity_id - The id of the entity to add a product to
   *  @param portal_name - The portal name
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param language - en, es, etc...
   *  @return - the data from the api
  */
  char * GETTokenProduct_selector( char *entity_id, char *portal_name, char *flatpack_id, char *language) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/product_selector",params);
  }


  /**
   * Provides a tokenised URL that allows a user to report incorrect entity information
   *
   *  @param entity_id - The unique Entity ID e.g. 379236608286720
   *  @param portal_name - The name of the portal that the user is coming from e.g. YourLocal
   *  @param language - The language to use to render the report path
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenReport( char *entity_id, char *portal_name, char *language, char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/report",params);
  }


  /**
   * Get a tokenised url for the review path
   *
   *  @param portal_name - The portal name
   *  @param entity_id
   *  @param language - en, es, etc...
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenReview( char *portal_name, char *entity_id, char *language, char *flatpack_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/review",params);
  }


  /**
   * Get a tokenised url for the testimonial path
   *
   *  @param portal_name - The portal name
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param language - en, es, etc...
   *  @param entity_id
   *  @param shorten_url
   *  @return - the data from the api
  */
  char * GETTokenTestimonial( char *portal_name, char *flatpack_id, char *language, char *entity_id, char *shorten_url) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"flatpack_id=");
    p = curl_easy_escape(curl,flatpack_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"shorten_url=");
    p = curl_easy_escape(curl,shorten_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/testimonial",params);
  }


  /**
   * The JaroWinklerDistance between two entities postal address objects
   *
   *  @param first_entity_id - The entity id of the first entity to be used in the postal address difference
   *  @param second_entity_id - The entity id of the second entity to be used in the postal address difference
   *  @return - the data from the api
  */
  char * GETToolsAddressdiff( char *first_entity_id, char *second_entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"first_entity_id=");
    p = curl_easy_escape(curl,first_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"second_entity_id=");
    p = curl_easy_escape(curl,second_entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/addressdiff",params);
  }


  /**
   * An API call to test crashing the server
   *
   *  @return - the data from the api
  */
  char * GETToolsCrash() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/tools/crash",params);
  }


  /**
   * Provide a method, a path and some data to run a load of curl commands and get emailed when complete
   *
   *  @param method - The method e.g. POST
   *  @param path - The relative api call e.g. /entity/phone
   *  @param filedata - A tab separated file for ingest
   *  @param email - Response email address e.g. dave@fender.com
   *  @return - the data from the api
  */
  char * POSTToolsCurl( char *method, char *path, char *filedata, char *email) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"method=");
    p = curl_easy_escape(curl,method,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/tools/curl",params);
  }


  /**
   * Use this call to get information (in HTML or JSON) about the data structure of given entity object (e.g. a phone number or an address)
   *
   *  @param object - The API call documentation is required for
   *  @param format - The format of the returned data eg. JSON or HTML
   *  @return - the data from the api
  */
  char * GETToolsDocs( char *object, char *format) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"object=");
    p = curl_easy_escape(curl,object,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"format=");
    p = curl_easy_escape(curl,format,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/docs",params);
  }


  /**
   * Format an address according to the rules of the country supplied
   *
   *  @param address - The address to format
   *  @param country - The country where the address is based
   *  @return - the data from the api
  */
  char * GETToolsFormatAddress( char *address, char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"address=");
    p = curl_easy_escape(curl,address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/format/address",params);
  }


  /**
   * Format a phone number according to the rules of the country supplied
   *
   *  @param number - The telephone number to format
   *  @param country - The country where the telephone number is based
   *  @param ignoreRegionCheck - If ture, we only check if the phone number is valid, ignoring country context
   *  @return - the data from the api
  */
  char * GETToolsFormatPhone( char *number, char *country, char *ignoreRegionCheck) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"number=");
    p = curl_easy_escape(curl,number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"ignoreRegionCheck=");
    p = curl_easy_escape(curl,ignoreRegionCheck,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/format/phone",params);
  }


  /**
   * Supply an address to geocode - returns lat/lon and accuracy
   *
   *  @param building_number
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param province
   *  @param postcode
   *  @param country
   *  @return - the data from the api
  */
  char * GETToolsGeocode( char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"building_number=");
    p = curl_easy_escape(curl,building_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address1=");
    p = curl_easy_escape(curl,address1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address2=");
    p = curl_easy_escape(curl,address2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address3=");
    p = curl_easy_escape(curl,address3,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"district=");
    p = curl_easy_escape(curl,district,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"town=");
    p = curl_easy_escape(curl,town,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"county=");
    p = curl_easy_escape(curl,county,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"province=");
    p = curl_easy_escape(curl,province,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcode=");
    p = curl_easy_escape(curl,postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/geocode",params);
  }


  /**
   * Given a spreadsheet ID, and a worksheet ID, add a row
   *
   *  @param spreadsheet_key - The key of the spreadsheet to edit
   *  @param worksheet_key - The key of the worksheet to edit - failure to provide this will assume worksheet with the label 'Sheet1'
   *  @param data - A comma separated list to add as cells
   *  @return - the data from the api
  */
  char * POSTToolsGooglesheetAdd_row( char *spreadsheet_key, char *worksheet_key, char *data) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"spreadsheet_key=");
    p = curl_easy_escape(curl,spreadsheet_key,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"worksheet_key=");
    p = curl_easy_escape(curl,worksheet_key,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/tools/googlesheet/add_row",params);
  }


  /**
   * Given a spreadsheet ID and the name of a worksheet identify the Google ID for the worksheet
   *
   *  @param spreadsheet_key - The key of the spreadsheet
   *  @param worksheet_name - The name/label of the worksheet to identify
   *  @return - the data from the api
  */
  char * POSTToolsGooglesheetWorksheet_id( char *spreadsheet_key, char *worksheet_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"spreadsheet_key=");
    p = curl_easy_escape(curl,spreadsheet_key,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"worksheet_name=");
    p = curl_easy_escape(curl,worksheet_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/tools/googlesheet/worksheet_id",params);
  }


  /**
   * Given some image data we can resize and upload the images
   *
   *  @param filedata - The image data to upload and resize
   *  @param type - The type of image to upload and resize
   *  @param image_dir - Set the destination directory of the generated images on S3. Only available when Image API is enabled.
   *  @return - the data from the api
  */
  char * POSTToolsImage( char *filedata, char *type, char *image_dir) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"type=");
    p = curl_easy_escape(curl,type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"image_dir=");
    p = curl_easy_escape(curl,image_dir,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/tools/image",params);
  }


  /**
   * Generate JSON in the format to generate Mashery's IODocs
   *
   *  @param mode - The HTTP method of the API call to document. e.g. GET
   *  @param path - The path of the API call to document e.g, /entity
   *  @param endpoint - The Mashery 'endpoint' to prefix to our API paths e.g. v1
   *  @param doctype - Mashery has two forms of JSON to describe API methods; one on github, the other on its customer dashboard
   *  @return - the data from the api
  */
  char * GETToolsIodocs( char *mode, char *path, char *endpoint, char *doctype) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"mode=");
    p = curl_easy_escape(curl,mode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"path=");
    p = curl_easy_escape(curl,path,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"endpoint=");
    p = curl_easy_escape(curl,endpoint,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"doctype=");
    p = curl_easy_escape(curl,doctype,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/iodocs",params);
  }


  /**
   * compile the supplied less with the standard Bootstrap less into a CSS file
   *
   *  @param less - The LESS code to compile
   *  @return - the data from the api
  */
  char * GETToolsLess( char *less) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"less=");
    p = curl_easy_escape(curl,less,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/less",params);
  }


  /**
   * Parse unstructured address data to fit our structured address objects
   *
   *  @param address
   *  @param postcode
   *  @param country
   *  @param normalise
   *  @return - the data from the api
  */
  char * GETToolsParseAddress( char *address, char *postcode, char *country, char *normalise) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"address=");
    p = curl_easy_escape(curl,address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcode=");
    p = curl_easy_escape(curl,postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"normalise=");
    p = curl_easy_escape(curl,normalise,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/parse/address",params);
  }


  /**
   * Ring the person and verify their account
   *
   *  @param to - The phone number to verify
   *  @param from - The phone number to call from
   *  @param pin - The pin to verify the phone number with
   *  @param twilio_voice - The language to read the verification in
   *  @param extension - The pin to verify the phone number with
   *  @return - the data from the api
  */
  char * GETToolsPhonecallVerify( char *to, char *from, char *pin, char *twilio_voice, char *extension) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"to=");
    p = curl_easy_escape(curl,to,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from=");
    p = curl_easy_escape(curl,from,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"pin=");
    p = curl_easy_escape(curl,pin,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"twilio_voice=");
    p = curl_easy_escape(curl,twilio_voice,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"extension=");
    p = curl_easy_escape(curl,extension,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/phonecall/verify",params);
  }


  /**
   * Return the phonetic representation of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsPhonetic( char *text) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"text=");
    p = curl_easy_escape(curl,text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/phonetic",params);
  }


  /**
   * Attempt to process a phone number, removing anything which is not a digit
   *
   *  @param number
   *  @return - the data from the api
  */
  char * GETToolsProcess_phone( char *number) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"number=");
    p = curl_easy_escape(curl,number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/process_phone",params);
  }


  /**
   * Fully process a string. This includes removing punctuation, stops words and stemming a string. Also returns the phonetic representation of this string.
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsProcess_string( char *text) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"text=");
    p = curl_easy_escape(curl,text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/process_string",params);
  }


  /**
   * Force refresh of search indexes
   *
   *  @return - the data from the api
  */
  char * GETToolsReindex() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/tools/reindex",params);
  }


  /**
   * Check to see if a supplied email address is valid
   *
   *  @param from - The phone number from which the SMS orginates
   *  @param to - The phone number to which the SMS is to be sent
   *  @param message - The message to be sent in the SMS
   *  @return - the data from the api
  */
  char * GETToolsSendsms( char *from, char *to, char *message) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"from=");
    p = curl_easy_escape(curl,from,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to=");
    p = curl_easy_escape(curl,to,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"message=");
    p = curl_easy_escape(curl,message,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/sendsms",params);
  }


  /**
   * Spider a single url looking for key facts
   *
   *  @param url
   *  @param pages
   *  @param country
   *  @param save
   *  @return - the data from the api
  */
  char * GETToolsSpider( char *url, char *pages, char *country, char *save) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"url=");
    p = curl_easy_escape(curl,url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"pages=");
    p = curl_easy_escape(curl,pages,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"save=");
    p = curl_easy_escape(curl,save,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/spider",params);
  }


  /**
   * Returns a stemmed version of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsStem( char *text) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"text=");
    p = curl_easy_escape(curl,text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/stem",params);
  }


  /**
   * Removes stopwords from a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsStopwords( char *text) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"text=");
    p = curl_easy_escape(curl,text,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/stopwords",params);
  }


  /**
   * Fetch the result of submitted data we sent to InfoGroup
   *
   *  @param syndication_submission_id - The syndication_submission_id to fetch info for
   *  @return - the data from the api
  */
  char * GETToolsSubmissionInfogroup( char *syndication_submission_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"syndication_submission_id=");
    p = curl_easy_escape(curl,syndication_submission_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/submission/infogroup",params);
  }


  /**
   * Fetch the entity and convert it to 118 Places CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicate118( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/118",params);
  }


  /**
   * Fetch the entity and convert it to Bing Ads CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateBingads( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/bingads",params);
  }


  /**
   * Fetch the entity and convert it to Bing Places CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateBingplaces( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/bingplaces",params);
  }


  /**
   * Fetch the entity and convert it to DnB TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateDnb( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/dnb",params);
  }


  /**
   * Fetch the entity and convert add it to arlington
   *
   *  @param entity_id - The entity_id to fetch
   *  @param reseller_masheryid - The reseller_masheryid
   *  @param destructive - Add the entity or simulate adding the entity
   *  @param data_filter - The level of filtering to apply to the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateEnablemedia( char *entity_id, char *reseller_masheryid, char *destructive, char *data_filter) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_masheryid=");
    p = curl_easy_escape(curl,reseller_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"data_filter=");
    p = curl_easy_escape(curl,data_filter,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/enablemedia",params);
  }


  /**
   * Fetch the entity and convert add it to Factual
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateFactual( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/factual",params);
  }


  /**
   * Fetch the entity and convert it to Factual CSV / TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateFactualcsv( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/factualcsv",params);
  }


  /**
   * Syndicate an entity to Foursquare
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateFoursquare( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/foursquare",params);
  }


  /**
   * Fetch the entity and convert it to TomTom XML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateGoogle( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/google",params);
  }


  /**
   * Fetch the entity and convert it to Infobel CSV / TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateInfobelcsv( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/infobelcsv",params);
  }


  /**
   * Fetch the entity and convert add it to InfoGroup
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateInfogroup( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/infogroup",params);
  }


  /**
   * Fetch the entity and convert add it to Judy's Book
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateJudysbook( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/judysbook",params);
  }


  /**
   * Fetch the entity and convert it to Google KML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateKml( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/kml",params);
  }


  /**
   * Syndicate database to localdatabase.com
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateLocaldatabase( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/localdatabase",params);
  }


  /**
   * Fetch the entity and convert it to Nokia NBS CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateNokia( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/nokia",params);
  }


  /**
   * Post an entity to OpenStreetMap
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateOsm( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/osm",params);
  }


  /**
   * Syndicate an entity to ThomsonLocal
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateThomsonlocal( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/thomsonlocal",params);
  }


  /**
   * Fetch the entity and convert it to TomTom XML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateTomtom( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/tomtom",params);
  }


  /**
   * Fetch the entity and convert it to YALWA csv
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateYalwa( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/yalwa",params);
  }


  /**
   * Fetch the entity and convert add it to Yassaaaabeeee!!
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateYasabe( char *entity_id, char *destructive) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destructive=");
    p = curl_easy_escape(curl,destructive,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/syndicate/yasabe",params);
  }


  /**
   * Test to see whether this supplied data would already match an entity
   *
   *  @param name
   *  @param building_number
   *  @param branch_name
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param province
   *  @param postcode
   *  @param country
   *  @param latitude
   *  @param longitude
   *  @param timezone
   *  @param telephone_number
   *  @param additional_telephone_number
   *  @param email
   *  @param website
   *  @param category_id
   *  @param category_type
   *  @param do_not_display
   *  @param referrer_url
   *  @param referrer_name
   *  @return - the data from the api
  */
  char * GETToolsTestmatch( char *name, char *building_number, char *branch_name, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *additional_telephone_number, char *email, char *website, char *category_id, char *category_type, char *do_not_display, char *referrer_url, char *referrer_name) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"building_number=");
    p = curl_easy_escape(curl,building_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"branch_name=");
    p = curl_easy_escape(curl,branch_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address1=");
    p = curl_easy_escape(curl,address1,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address2=");
    p = curl_easy_escape(curl,address2,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"address3=");
    p = curl_easy_escape(curl,address3,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"district=");
    p = curl_easy_escape(curl,district,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"town=");
    p = curl_easy_escape(curl,town,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"county=");
    p = curl_easy_escape(curl,county,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"province=");
    p = curl_easy_escape(curl,province,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"postcode=");
    p = curl_easy_escape(curl,postcode,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"latitude=");
    p = curl_easy_escape(curl,latitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"longitude=");
    p = curl_easy_escape(curl,longitude,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"timezone=");
    p = curl_easy_escape(curl,timezone,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"telephone_number=");
    p = curl_easy_escape(curl,telephone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"additional_telephone_number=");
    p = curl_easy_escape(curl,additional_telephone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"website=");
    p = curl_easy_escape(curl,website,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_id=");
    p = curl_easy_escape(curl,category_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"category_type=");
    p = curl_easy_escape(curl,category_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"do_not_display=");
    p = curl_easy_escape(curl,do_not_display,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_url=");
    p = curl_easy_escape(curl,referrer_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"referrer_name=");
    p = curl_easy_escape(curl,referrer_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/testmatch",params);
  }


  /**
   * Send a transactional email via Adestra or other email provider
   *
   *  @param email_id - The ID of the email to send
   *  @param destination_email - The email address to send to
   *  @param email_supplier - The email supplier
   *  @return - the data from the api
  */
  char * GETToolsTransactional_email( char *email_id, char *destination_email, char *email_supplier) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email_id=");
    p = curl_easy_escape(curl,email_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"destination_email=");
    p = curl_easy_escape(curl,destination_email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email_supplier=");
    p = curl_easy_escape(curl,email_supplier,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/transactional_email",params);
  }


  /**
   * Upload a file to our asset server and return the url
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTToolsUpload( char *filedata) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"filedata=");
    p = curl_easy_escape(curl,filedata,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/tools/upload",params);
  }


  /**
   * Find a canonical URL from a supplied URL
   *
   *  @param url - The url to process
   *  @param max_redirects - The maximum number of reirects
   *  @return - the data from the api
  */
  char * GETToolsUrl_details( char *url, char *max_redirects) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"url=");
    p = curl_easy_escape(curl,url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"max_redirects=");
    p = curl_easy_escape(curl,max_redirects,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/url_details",params);
  }


  /**
   * Check to see if a supplied email address is valid
   *
   *  @param email_address - The email address to validate
   *  @return - the data from the api
  */
  char * GETToolsValidate_email( char *email_address) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email_address=");
    p = curl_easy_escape(curl,email_address,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/validate_email",params);
  }


  /**
   * Calls a number to make sure it is active
   *
   *  @param phone_number - The phone number to validate
   *  @param country - The country code of the phone number to be validated
   *  @return - the data from the api
  */
  char * GETToolsValidate_phone( char *phone_number, char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"phone_number=");
    p = curl_easy_escape(curl,phone_number,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/validate_phone",params);
  }


  /**
   * Deleting a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * DELETETraction( char *traction_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"traction_id=");
    p = curl_easy_escape(curl,traction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/traction",params);
  }


  /**
   * Fetching a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * GETTraction( char *traction_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"traction_id=");
    p = curl_easy_escape(curl,traction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/traction",params);
  }


  /**
   * Update/Add a traction
   *
   *  @param traction_id
   *  @param trigger_type
   *  @param action_type
   *  @param country
   *  @param email_addresses
   *  @param title
   *  @param body
   *  @param api_method
   *  @param api_url
   *  @param api_params
   *  @param active
   *  @param reseller_masheryid
   *  @param publisher_masheryid
   *  @param description
   *  @return - the data from the api
  */
  char * POSTTraction( char *traction_id, char *trigger_type, char *action_type, char *country, char *email_addresses, char *title, char *body, char *api_method, char *api_url, char *api_params, char *active, char *reseller_masheryid, char *publisher_masheryid, char *description) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"traction_id=");
    p = curl_easy_escape(curl,traction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"trigger_type=");
    p = curl_easy_escape(curl,trigger_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"email_addresses=");
    p = curl_easy_escape(curl,email_addresses,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"title=");
    p = curl_easy_escape(curl,title,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"body=");
    p = curl_easy_escape(curl,body,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"api_method=");
    p = curl_easy_escape(curl,api_method,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"api_url=");
    p = curl_easy_escape(curl,api_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"api_params=");
    p = curl_easy_escape(curl,api_params,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_masheryid=");
    p = curl_easy_escape(curl,reseller_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"publisher_masheryid=");
    p = curl_easy_escape(curl,publisher_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"description=");
    p = curl_easy_escape(curl,description,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/traction",params);
  }


  /**
   * Fetching active tractions
   *
   *  @return - the data from the api
  */
  char * GETTractionActive() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/traction/active",params);
  }


  /**
   * Create a new transaction
   *
   *  @param entity_id
   *  @param user_id
   *  @param basket_total
   *  @param basket
   *  @param currency
   *  @param notes
   *  @return - the data from the api
  */
  char * PUTTransaction( char *entity_id, char *user_id, char *basket_total, char *basket, char *currency, char *notes) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"basket_total=");
    p = curl_easy_escape(curl,basket_total,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"basket=");
    p = curl_easy_escape(curl,basket,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"currency=");
    p = curl_easy_escape(curl,currency,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"notes=");
    p = curl_easy_escape(curl,notes,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/transaction",params);
  }


  /**
   * Given a transaction_id retrieve information on it
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * GETTransaction( char *transaction_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"transaction_id=");
    p = curl_easy_escape(curl,transaction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/transaction",params);
  }


  /**
   * Set a transactions status to authorised
   *
   *  @param transaction_id
   *  @param paypal_getexpresscheckoutdetails
   *  @return - the data from the api
  */
  char * POSTTransactionAuthorised( char *transaction_id, char *paypal_getexpresscheckoutdetails) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"transaction_id=");
    p = curl_easy_escape(curl,transaction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"paypal_getexpresscheckoutdetails=");
    p = curl_easy_escape(curl,paypal_getexpresscheckoutdetails,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/transaction/authorised",params);
  }


  /**
   * Given a transaction_id retrieve information on it
   *
   *  @param paypal_transaction_id
   *  @return - the data from the api
  */
  char * GETTransactionBy_paypal_transaction_id( char *paypal_transaction_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"paypal_transaction_id=");
    p = curl_easy_escape(curl,paypal_transaction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/transaction/by_paypal_transaction_id",params);
  }


  /**
   * Set a transactions status to cancelled
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * POSTTransactionCancelled( char *transaction_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"transaction_id=");
    p = curl_easy_escape(curl,transaction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/transaction/cancelled",params);
  }


  /**
   * Set a transactions status to complete
   *
   *  @param transaction_id
   *  @param paypal_doexpresscheckoutpayment
   *  @param user_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTTransactionComplete( char *transaction_id, char *paypal_doexpresscheckoutpayment, char *user_id, char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"transaction_id=");
    p = curl_easy_escape(curl,transaction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"paypal_doexpresscheckoutpayment=");
    p = curl_easy_escape(curl,paypal_doexpresscheckoutpayment,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/transaction/complete",params);
  }


  /**
   * Set a transactions status to inprogess
   *
   *  @param transaction_id
   *  @param paypal_setexpresscheckout
   *  @return - the data from the api
  */
  char * POSTTransactionInprogress( char *transaction_id, char *paypal_setexpresscheckout) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"transaction_id=");
    p = curl_easy_escape(curl,transaction_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"paypal_setexpresscheckout=");
    p = curl_easy_escape(curl,paypal_setexpresscheckout,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/transaction/inprogress",params);
  }


  /**
   * Update user based on email address or social_network/social_network_id
   *
   *  @param email
   *  @param user_id
   *  @param first_name
   *  @param last_name
   *  @param active
   *  @param last_flatpack - Last visited flatpack (used for admin deep linking)
   *  @param trust
   *  @param creation_date
   *  @param user_type
   *  @param social_network
   *  @param social_network_id
   *  @param reseller_admin_masheryid
   *  @param group_id
   *  @param admin_upgrader
   *  @param opt_in_marketing
   *  @return - the data from the api
  */
  char * POSTUser( char *email, char *user_id, char *first_name, char *last_name, char *active, char *last_flatpack, char *trust, char *creation_date, char *user_type, char *social_network, char *social_network_id, char *reseller_admin_masheryid, char *group_id, char *admin_upgrader, char *opt_in_marketing) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"first_name=");
    p = curl_easy_escape(curl,first_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"last_name=");
    p = curl_easy_escape(curl,last_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"last_flatpack=");
    p = curl_easy_escape(curl,last_flatpack,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"trust=");
    p = curl_easy_escape(curl,trust,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"creation_date=");
    p = curl_easy_escape(curl,creation_date,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_type=");
    p = curl_easy_escape(curl,user_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"social_network=");
    p = curl_easy_escape(curl,social_network,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"social_network_id=");
    p = curl_easy_escape(curl,social_network_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"reseller_admin_masheryid=");
    p = curl_easy_escape(curl,reseller_admin_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"admin_upgrader=");
    p = curl_easy_escape(curl,admin_upgrader,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"opt_in_marketing=");
    p = curl_easy_escape(curl,opt_in_marketing,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/user",params);
  }


  /**
   * With a unique ID address an user can be retrieved
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETUser( char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user",params);
  }


  /**
   * Is this user allowed to edit this entity
   *
   *  @param entity_id
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETUserAllowed_to_edit( char *entity_id, char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/allowed_to_edit",params);
  }


  /**
   * With a unique email address an user can be retrieved
   *
   *  @param email
   *  @return - the data from the api
  */
  char * GETUserBy_email( char *email) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"email=");
    p = curl_easy_escape(curl,email,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/by_email",params);
  }


  /**
   * Returns all the users that match the supplied group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * GETUserBy_groupid( char *group_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"group_id=");
    p = curl_easy_escape(curl,group_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/by_groupid",params);
  }


  /**
   * Returns all the users that match the supplied reseller_admin_masheryid
   *
   *  @param reseller_admin_masheryid
   *  @return - the data from the api
  */
  char * GETUserBy_reseller_admin_masheryid( char *reseller_admin_masheryid) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"reseller_admin_masheryid=");
    p = curl_easy_escape(curl,reseller_admin_masheryid,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/by_reseller_admin_masheryid",params);
  }


  /**
   * With a unique ID address an user can be retrieved
   *
   *  @param name
   *  @param id
   *  @return - the data from the api
  */
  char * GETUserBy_social_media( char *name, char *id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"name=");
    p = curl_easy_escape(curl,name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"id=");
    p = curl_easy_escape(curl,id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/by_social_media",params);
  }


  /**
   * Downgrade an existing user
   *
   *  @param user_id
   *  @param user_type
   *  @return - the data from the api
  */
  char * POSTUserDowngrade( char *user_id, char *user_type) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"user_type=");
    p = curl_easy_escape(curl,user_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/user/downgrade",params);
  }


  /**
   * Removes group_admin privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * POSTUserGroup_admin_remove( char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/user/group_admin_remove",params);
  }


  /**
   * Log user activities into MariaDB
   *
   *  @param user_id
   *  @param action_type
   *  @param domain
   *  @param time
   *  @return - the data from the api
  */
  char * POSTUserLog_activity( char *user_id, char *action_type, char *domain, char *time) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"domain=");
    p = curl_easy_escape(curl,domain,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"time=");
    p = curl_easy_escape(curl,time,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/user/log_activity",params);
  }


  /**
   * List user activity times within given date range (between inclusive from and exclusive to)
   *
   *  @param user_id
   *  @param action_type
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * GETUserLog_activityList_time( char *user_id, char *action_type, char *from, char *to) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"from=");
    p = curl_easy_escape(curl,from,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"to=");
    p = curl_easy_escape(curl,to,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/log_activity/list_time",params);
  }


  /**
   * Retrieve list of entities that the user manages
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETUserManaged_entities( char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/user/managed_entities",params);
  }


  /**
   * Removes reseller privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * POSTUserReseller_remove( char *user_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/user/reseller_remove",params);
  }


  /**
   * Deletes a specific social network from a user
   *
   *  @param user_id
   *  @param social_network
   *  @return - the data from the api
  */
  char * DELETEUserSocial_network( char *user_id, char *social_network) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"user_id=");
    p = curl_easy_escape(curl,user_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"social_network=");
    p = curl_easy_escape(curl,social_network,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("DELETE","/user/social_network",params);
  }


  /**
   * Shows what would be emitted by a view, given a document
   *
   *  @param database - the database being worked on e.g. entities
   *  @param designdoc - the design document containing the view e.g. _design/report
   *  @param view - the name of the view to be tested e.g. bydate
   *  @param doc - the JSON document to be analysed e.g. {}
   *  @return - the data from the api
  */
  char * GETViewhelper( char *database, char *designdoc, char *view, char *doc) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"database=");
    p = curl_easy_escape(curl,database,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"designdoc=");
    p = curl_easy_escape(curl,designdoc,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"view=");
    p = curl_easy_escape(curl,view,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"doc=");
    p = curl_easy_escape(curl,doc,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/viewhelper",params);
  }


  /**
   * Converts an Entity into webcard JSON and then doing a PUT /webcard/json
   *
   *  @param entity_id - The entity to create on webcard
   *  @return - the data from the api
  */
  char * POSTWebcard( char *entity_id) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"entity_id=");
    p = curl_easy_escape(curl,entity_id,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/webcard",params);
  }





