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
   * Confirms that the API is active, and returns the current version number
   *
   *  @return - the data from the api
  */
  char * getStatus() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/status",params);
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
  char * getLogo( char *a, char *b, char *c, char *d) {
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
  char * putLogo( char *a) {
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
   * Uploads a CSV file of known format and bulk inserts into DB
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * postEntityBulkCsv( char *filedata) {
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
   * Uploads a JSON file of known format and bulk inserts into DB
   *
   *  @param data
   *  @return - the data from the api
  */
  char * postEntityBulkJson( char *data) {
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
    return doCurl("POST","/entity/bulk/json",params);
  }


  /**
   * Shows the current status of a bulk upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * getEntityBulkCsvStatus( char *upload_id) {
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
   * Shows the current status of a bulk JSON upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * getEntityBulkJsonStatus( char *upload_id) {
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
   * This entity isn't really supported anymore. You probably want PUT /business. Only to be used for testing.
   *
   *  @param type
   *  @param scope
   *  @param country
   *  @param trust
   *  @param our_data
   *  @return - the data from the api
  */
  char * putEntity( char *type, char *scope, char *country, char *trust, char *our_data) {
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
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID
   *  @return - the data from the api
  */
  char * getEntityBy_supplier_id( char *supplier_id) {
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
   * Search for matching entities
   *
   *  @param what
   *  @param entity_name
   *  @param where
   *  @param per_page
   *  @param page
   *  @param longitude
   *  @param latitude
   *  @param country
   *  @param language
   *  @return - the data from the api
  */
  char * getEntitySearch( char *what, char *entity_name, char *where, char *per_page, char *page, char *longitude, char *latitude, char *country, char *language) {
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
    strcat(params,"entity_name=");
    p = curl_easy_escape(curl,entity_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"where=");
    p = curl_easy_escape(curl,where,0);
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
    return doCurl("GET","/entity/search",params);
  }


  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param where - The location to get results for. E.g. Dublin e.g. Dublin
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntitySearchWhatBylocation( char *what, char *where, char *per_page, char *page, char *country, char *language) {
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
    return doCurl("GET","/entity/search/what/bylocation",params);
  }


  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param latitude_1 - Latitude of first point in bounding box e.g. 53.396842
   *  @param longitude_1 - Longitude of first point in bounding box e.g. -6.37619
   *  @param latitude_2 - Latitude of second point in bounding box e.g. 53.290463
   *  @param longitude_2 - Longitude of second point in bounding box e.g. -6.207275
   *  @param per_page
   *  @param page
   *  @param country - A valid ISO 3166 country code e.g. ie
   *  @param language
   *  @return - the data from the api
  */
  char * getEntitySearchWhatByboundingbox( char *what, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *per_page, char *page, char *country, char *language) {
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
    return doCurl("GET","/entity/search/what/byboundingbox",params);
  }


  /**
   * Search for matching entities
   *
   *  @param who
   *  @param latitude_1
   *  @param longitude_1
   *  @param latitude_2
   *  @param longitude_2
   *  @param per_page
   *  @param page
   *  @param country
   *  @return - the data from the api
  */
  char * getEntitySearchWhoByboundingbox( char *who, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *per_page, char *page, char *country) {
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
    return doCurl("GET","/entity/search/who/byboundingbox",params);
  }


  /**
   * Search for matching entities
   *
   *  @param who - Company Name e.g. Starbucks
   *  @param where - The location to get results for. E.g. Dublin e.g. Dublin
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @return - the data from the api
  */
  char * getEntitySearchWhoBylocation( char *who, char *where, char *per_page, char *page, char *country) {
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
    return doCurl("GET","/entity/search/who/bylocation",params);
  }


  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param per_page - Number of results returned per page
   *  @param page - The page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntitySearchWhat( char *what, char *per_page, char *page, char *country, char *language) {
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
    return doCurl("GET","/entity/search/what",params);
  }


  /**
   * Search for matching entities
   *
   *  @param who - Company name e.g. Starbucks
   *  @param per_page - How many results per page
   *  @param page - What page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @return - the data from the api
  */
  char * getEntitySearchWho( char *who, char *per_page, char *page, char *country) {
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
    return doCurl("GET","/entity/search/who",params);
  }


  /**
   * Search for matching entities
   *
   *  @param where - Location to search for results. E.g. Dublin e.g. Dublin
   *  @param per_page - How many results per page
   *  @param page - What page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntitySearchBylocation( char *where, char *per_page, char *page, char *country, char *language) {
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
    return doCurl("GET","/entity/search/bylocation",params);
  }


  /**
   * Search for matching entities
   *
   *  @param latitude_1
   *  @param longitude_1
   *  @param latitude_2
   *  @param longitude_2
   *  @param per_page
   *  @param page
   *  @param country
   *  @param language
   *  @return - the data from the api
  */
  char * getEntitySearchByboundingbox( char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *per_page, char *page, char *country, char *language) {
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
    return doCurl("GET","/entity/search/byboundingbox",params);
  }


  /**
   * Search for matching entities that are advertisers and return a random selection upto the limit requested
   *
   *  @param tag - The word or words the advertiser is to appear for in searches
   *  @param where - The location to get results for. E.g. Dublin
   *  @param limit - The number of advertisers that are to be returned
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntityAdvertisers( char *tag, char *where, char *limit, char *country, char *language) {
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
    return doCurl("GET","/entity/advertisers",params);
  }


  /**
   * Allows a whole entity to be pulled from the datastore by its unique id
   *
   *  @param entity_id - The unique entity ID e.g. 379236608286720
   *  @return - the data from the api
  */
  char * getEntity( char *entity_id) {
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
    return doCurl("GET","/entity",params);
  }


  /**
   * Get all entiies claimed by a specific user
   *
   *  @param user_id - The unique user ID of the user with claimed entities e.g. 379236608286720
   *  @return - the data from the api
  */
  char * getEntityBy_user_id( char *user_id) {
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
    return doCurl("GET","/entity/by_user_id",params);
  }


  /**
   * Allows a list of available revisions to be returned by its entity id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * getEntityRevisions( char *entity_id) {
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
  char * getEntityRevisionsByRevisionID( char *entity_id, char *revision_id) {
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
   * Separates an entity into two distinct entities 
   *
   *  @param entity_id
   *  @param supplier_masheryid
   *  @param supplier_id
   *  @return - the data from the api
  */
  char * postEntityUnmerge( char *entity_id, char *supplier_masheryid, char *supplier_id) {
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
    return doCurl("POST","/entity/unmerge",params);
  }


  /**
   * Fetches the changelog documents that match the given entity_id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * getEntityChangelog( char *entity_id) {
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
   * Merge two entities into one
   *
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * postEntityMerge( char *from, char *to) {
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
    return doCurl("POST","/entity/merge",params);
  }


  /**
   * Force refresh of search indexes
   *
   *  @return - the data from the api
  */
  char * getToolsReindex() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/tools/reindex",params);
  }


  /**
   * Update entities that use an old category ID to a new one
   *
   *  @param from
   *  @param to
   *  @param limit
   *  @return - the data from the api
  */
  char * postEntityMigrate_category( char *from, char *to, char *limit) {
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
   * Create a new business entity with all it's objects
   *
   *  @param name
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param postcode
   *  @param country
   *  @param latitude
   *  @param longitude
   *  @param timezone
   *  @param telephone_number
   *  @param email
   *  @param website
   *  @param category_id
   *  @param category_type
   *  @param do_not_display
   *  @return - the data from the api
  */
  char * putBusiness( char *name, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *email, char *website, char *category_id, char *category_type, char *do_not_display) {
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
    return doCurl("PUT","/business",params);
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
  char * postEntityAdvertiserTag( char *gen_id, char *entity_id, char *language, char *tags_to_add, char *tags_to_remove) {
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
   * Find a location from cache or cloudant depending if it is in the cache
   *
   *  @param string
   *  @param country
   *  @return - the data from the api
  */
  char * getLookupLocation( char *string, char *country) {
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
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/lookup/location",params);
  }


  /**
   * Find a category from cache or cloudant depending if it is in the cache
   *
   *  @param string - A string to search against, E.g. Plumbers
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getLookupCategory( char *string, char *language) {
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
  char * getLookupLegacyCategory( char *id, char *type) {
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
   * Find all the parents locations of the selected location
   *
   *  @param location_id
   *  @return - the data from the api
  */
  char * getLookupLocationParents( char *location_id) {
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
    return doCurl("GET","/lookup/location/parents",params);
  }


  /**
   * Find all the child locations of the selected location
   *
   *  @param location_id
   *  @param resolution
   *  @return - the data from the api
  */
  char * getLookupLocationChildren( char *location_id, char *resolution) {
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
    strcat(params,"resolution=");
    p = curl_easy_escape(curl,resolution,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/lookup/location/children",params);
  }


  /**
   * With a known entity id, a name can be updated.
   *
   *  @param entity_id
   *  @param name
   *  @param formal_name
   *  @return - the data from the api
  */
  char * postEntityName( char *entity_id, char *name, char *formal_name) {
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
    return doCurl("POST","/entity/name",params);
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
  char * postEntityBackground( char *entity_id, char *number_of_employees, char *turnover, char *net_profit, char *vat_number, char *duns_number, char *registered_company_number) {
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
  char * postEntityEmployee( char *entity_id, char *title, char *forename, char *surname, char *job_title, char *description, char *email, char *phone_number) {
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
  char * deleteEntityEmployee( char *entity_id, char *gen_id) {
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
   * Allows a new phone object to be added to a specified entity. A new object id will be calculated and returned to you if successful.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @return - the data from the api
  */
  char * postEntityPhone( char *entity_id, char *number, char *description) {
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
    return doCurl("POST","/entity/phone",params);
  }


  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityPhone( char *entity_id, char *gen_id) {
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
   * With a known entity id, an fax object can be added.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @return - the data from the api
  */
  char * postEntityFax( char *entity_id, char *number, char *description) {
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
   * Allows a fax object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityFax( char *entity_id, char *gen_id) {
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
   * With a known category id, an category object can be added.
   *
   *  @param category_id
   *  @param language
   *  @param name
   *  @return - the data from the api
  */
  char * putCategory( char *category_id, char *language, char *name) {
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
   * With a known category id, a mapping object can be added.
   *
   *  @param category_id
   *  @param type
   *  @param id
   *  @param name
   *  @return - the data from the api
  */
  char * postCategoryMappings( char *category_id, char *type, char *id, char *name) {
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
   * With a known category id, an synonym object can be added.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * postCategorySynonym( char *category_id, char *synonym, char *language) {
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
  char * deleteCategorySynonym( char *category_id, char *synonym, char *language) {
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
   * Allows a category object to merged with another
   *
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * postCategoryMerge( char *from, char *to) {
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
   * Returns the supplied wolf category object by fetching the supplied category_id from our categories object.
   *
   *  @param category_id
   *  @return - the data from the api
  */
  char * getCategory( char *category_id) {
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
   * With a known entity id, an category object can be added.
   *
   *  @param entity_id
   *  @param category_id
   *  @param category_type
   *  @return - the data from the api
  */
  char * postEntityCategory( char *entity_id, char *category_id, char *category_type) {
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
  char * deleteEntityCategory( char *entity_id, char *gen_id) {
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
   * With a known entity id, a geopoint can be updated.
   *
   *  @param entity_id
   *  @param longitude
   *  @param latitude
   *  @param accuracy
   *  @return - the data from the api
  */
  char * postEntityGeopoint( char *entity_id, char *longitude, char *latitude, char *accuracy) {
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
   * Find all matches by phone number and then return all matches that also match company name and location. Default location_strictness is defined in Km and the default is set to 0.2 (200m)
   *
   *  @param phone
   *  @param company_name
   *  @param latitude
   *  @param longitude
   *  @param country
   *  @param name_strictness
   *  @param location_strictness
   *  @return - the data from the api
  */
  char * getMatchByphone( char *phone, char *company_name, char *latitude, char *longitude, char *country, char *name_strictness, char *location_strictness) {
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
    strcat(params,"company_name=");
    p = curl_easy_escape(curl,company_name,0);
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
    strcat(params,"country=");
    p = curl_easy_escape(curl,country,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"name_strictness=");
    p = curl_easy_escape(curl,name_strictness,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location_strictness=");
    p = curl_easy_escape(curl,location_strictness,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/match/byphone",params);
  }


  /**
   * Find all matches by location and then return all matches that also match company name. Default location_strictness is set to 7, which equates to +/- 20m
   *
   *  @param company_name
   *  @param latitude
   *  @param longitude
   *  @param name_strictness
   *  @param location_strictness
   *  @return - the data from the api
  */
  char * getMatchBylocation( char *company_name, char *latitude, char *longitude, char *name_strictness, char *location_strictness) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    strcat(params,"company_name=");
    p = curl_easy_escape(curl,company_name,0);
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
    strcat(params,"name_strictness=");
    p = curl_easy_escape(curl,name_strictness,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"location_strictness=");
    p = curl_easy_escape(curl,location_strictness,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/match/bylocation",params);
  }


  /**
   * Removes stopwords from a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsStopwords( char *text) {
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
   * Returns a stemmed version of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsStem( char *text) {
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
   * Return the phonetic representation of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsPhonetic( char *text) {
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
   * Fully process a string. This includes removing punctuation, stops words and stemming a string. Also returns the phonetic representation of this string.
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsProcess_string( char *text) {
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
   * Attempt to process a phone number, removing anything which is not a digit
   *
   *  @param number
   *  @return - the data from the api
  */
  char * getToolsProcess_phone( char *number) {
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
   * Spider a single url looking for key facts
   *
   *  @param url
   *  @param pages
   *  @param country
   *  @return - the data from the api
  */
  char * getToolsSpider( char *url, char *pages, char *country) {
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
    return doCurl("GET","/tools/spider",params);
  }


  /**
   * Supply an address to geocode - returns lat/lon and accuracy
   *
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param postcode
   *  @param country
   *  @return - the data from the api
  */
  char * getToolsGeocode( char *address1, char *address2, char *address3, char *district, char *town, char *county, char *postcode, char *country) {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
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
   * Generate JSON in the format to generate Mashery's IODocs
   *
   *  @param mode - The HTTP method of the API call to document. e.g. GET
   *  @param path - The path of the API call to document e.g, /entity
   *  @param endpoint - The Mashery 'endpoint' to prefix to our API paths e.g. v1
   *  @param doctype - Mashery has two forms of JSON to describe API methods; one on github, the other on its customer dashboard
   *  @return - the data from the api
  */
  char * getToolsIodocs( char *mode, char *path, char *endpoint, char *doctype) {
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
   * Use this call to get information (in HTML or JSON) about the data structure of given entity object (e.g. a phone number or an address)
   *
   *  @param object - The API call documentation is required for
   *  @param format - The format of the returned data eg. JSON or HTML
   *  @return - the data from the api
  */
  char * getToolsDocs( char *object, char *format) {
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
   * Format a phone number according to the rules of the country supplied
   *
   *  @param number - The telephone number to format
   *  @param country - The country where the telephone number is based
   *  @return - the data from the api
  */
  char * getToolsFormatPhone( char *number, char *country) {
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
    return doCurl("GET","/tools/format/phone",params);
  }


  /**
   * Format an address according to the rules of the country supplied
   *
   *  @param address - The address to format
   *  @param country - The country where the address is based
   *  @return - the data from the api
  */
  char * getToolsFormatAddress( char *address, char *country) {
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
   * Check to see if a supplied email address is valid
   *
   *  @param email_address - The email address to validate
   *  @return - the data from the api
  */
  char * getToolsValidate_email( char *email_address) {
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
   * compile the supplied less with the standard Bootstrap less into a CSS file
   *
   *  @param less - The LESS code to compile
   *  @return - the data from the api
  */
  char * getToolsLess( char *less) {
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
   * replace some text parameters with some entity details
   *
   *  @param entity_id - The entity to pull for replacements
   *  @param string - The string full of parameters
   *  @return - the data from the api
  */
  char * getToolsReplace( char *entity_id, char *string) {
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
    strcat(params,"string=");
    p = curl_easy_escape(curl,string,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/tools/replace",params);
  }


  /**
   * Check to see if a supplied email address is valid
   *
   *  @param from - The phone number from which the SMS orginates
   *  @param to - The phone number to which the SMS is to be sent
   *  @param message - The message to be sent in the SMS
   *  @return - the data from the api
  */
  char * getToolsSendsms( char *from, char *to, char *message) {
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
   * Given a spreadsheet id add a row
   *
   *  @param spreadsheet_key - The key of the spreadsheet to edit
   *  @param data - A comma separated list to add as cells
   *  @return - the data from the api
  */
  char * postToolsGooglesheetAdd_row( char *spreadsheet_key, char *data) {
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
    strcat(params,"data=");
    p = curl_easy_escape(curl,data,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/tools/googlesheet/add_row",params);
  }


  /**
   * With a known entity id, an invoice_address object can be updated.
   *
   *  @param entity_id
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param postcode
   *  @param address_type
   *  @return - the data from the api
  */
  char * postEntityInvoice_address( char *entity_id, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *postcode, char *address_type) {
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
  char * deleteEntityInvoice_address( char *entity_id) {
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
   * With a known entity id, an tag object can be added.
   *
   *  @param entity_id
   *  @param tag
   *  @param language
   *  @return - the data from the api
  */
  char * postEntityTag( char *entity_id, char *tag, char *language) {
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
   * Allows a tag object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityTag( char *entity_id, char *gen_id) {
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
   * Create/Update a postal address
   *
   *  @param entity_id
   *  @param address1
   *  @param address2
   *  @param address3
   *  @param district
   *  @param town
   *  @param county
   *  @param postcode
   *  @param address_type
   *  @param do_not_display
   *  @return - the data from the api
  */
  char * postEntityPostal_address( char *entity_id, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *postcode, char *address_type, char *do_not_display) {
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
   * With a known entity id, a advertiser is added
   *
   *  @param entity_id
   *  @param tags
   *  @param locations
   *  @param max_tags
   *  @param max_locations
   *  @param expiry_date
   *  @param is_national
   *  @param language
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * postEntityAdvertiserCreate( char *entity_id, char *tags, char *locations, char *max_tags, char *max_locations, char *expiry_date, char *is_national, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id) {
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
   * With a known entity id, an advertiser is updated
   *
   *  @param entity_id
   *  @param tags
   *  @param locations
   *  @param extra_tags
   *  @param extra_locations
   *  @param is_national
   *  @param language
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * postEntityAdvertiserUpsell( char *entity_id, char *tags, char *locations, char *extra_tags, char *extra_locations, char *is_national, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id) {
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
   * Expires an advertiser from and entity
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @return - the data from the api
  */
  char * postEntityAdvertiserCancel( char *entity_id, char *publisher_id, char *reseller_ref, char *reseller_agent_id) {
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
   * Renews an advertiser from an entity
   *
   *  @param entity_id
   *  @param expiry_date
   *  @param publisher_id
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @return - the data from the api
  */
  char * postEntityAdvertiserRenew( char *entity_id, char *expiry_date, char *publisher_id, char *reseller_ref, char *reseller_agent_id) {
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
   * Allows an advertiser object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityAdvertiser( char *entity_id, char *gen_id) {
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
   * Adds/removes locations
   *
   *  @param entity_id
   *  @param gen_id
   *  @param locations_to_add
   *  @param locations_to_remove
   *  @return - the data from the api
  */
  char * postEntityAdvertiserLocation( char *entity_id, char *gen_id, char *locations_to_add, char *locations_to_remove) {
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
   * With a known entity id, an email address object can be added.
   *
   *  @param entity_id
   *  @param email_address
   *  @param email_description
   *  @return - the data from the api
  */
  char * postEntityEmail( char *entity_id, char *email_address, char *email_description) {
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
   * Allows a email object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityEmail( char *entity_id, char *gen_id) {
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
   * With a known entity id, a website object can be added.
   *
   *  @param entity_id
   *  @param website_url
   *  @param display_url
   *  @param website_description
   *  @return - the data from the api
  */
  char * postEntityWebsite( char *entity_id, char *website_url, char *display_url, char *website_description) {
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
    return doCurl("POST","/entity/website",params);
  }


  /**
   * Allows a website object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityWebsite( char *entity_id, char *gen_id) {
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
    return doCurl("DELETE","/entity/website",params);
  }


  /**
   * With a known entity id, a image object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param image_name
   *  @return - the data from the api
  */
  char * postEntityImage( char *entity_id, char *filedata, char *image_name) {
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
  char * deleteEntityImage( char *entity_id, char *gen_id) {
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
   * Read a location with the supplied ID in the locations reference database.
   *
   *  @param location_id
   *  @return - the data from the api
  */
  char * getLocation( char *location_id) {
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
   * Read multiple locations with the supplied ID in the locations reference database.
   *
   *  @param location_ids
   *  @return - the data from the api
  */
  char * getLocationMultiple( char *location_ids) {
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
   * Create/update a new location entity with the supplied ID in the locations reference database.
   *
   *  @param location_id
   *  @param name
   *  @param formal_name
   *  @param latitude
   *  @param longitude
   *  @param resolution
   *  @param country
   *  @param population
   *  @param description
   *  @param timezone
   *  @param is_duplicate
   *  @param is_default
   *  @param parent_town
   *  @param parent_county
   *  @param parent_province
   *  @param parent_region
   *  @param parent_neighbourhood
   *  @param parent_district
   *  @param postalcode
   *  @return - the data from the api
  */
  char * postLocation( char *location_id, char *name, char *formal_name, char *latitude, char *longitude, char *resolution, char *country, char *population, char *description, char *timezone, char *is_duplicate, char *is_default, char *parent_town, char *parent_county, char *parent_province, char *parent_region, char *parent_neighbourhood, char *parent_district, char *postalcode) {
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
    strcat(params,"is_duplicate=");
    p = curl_easy_escape(curl,is_duplicate,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"is_default=");
    p = curl_easy_escape(curl,is_default,0);
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
    return doCurl("POST","/location",params);
  }


  /**
   * Add a new synonym to a known location
   *
   *  @param location_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * postLocationSynonym( char *location_id, char *synonym, char *language) {
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
    return doCurl("POST","/location/synonym",params);
  }


  /**
   * Remove a new synonym from a known location
   *
   *  @param location_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * deleteLocationSynonym( char *location_id, char *synonym, char *language) {
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
    return doCurl("DELETE","/location/synonym",params);
  }


  /**
   * Add a new source to a known location
   *
   *  @param location_id
   *  @param type
   *  @param url
   *  @param ref
   *  @return - the data from the api
  */
  char * postLocationSource( char *location_id, char *type, char *url, char *ref) {
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
    strcat(params,"url=");
    p = curl_easy_escape(curl,url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"ref=");
    p = curl_easy_escape(curl,ref,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/location/source",params);
  }


  /**
   * With a known entity id, a status object can be updated.
   *
   *  @param entity_id
   *  @param status
   *  @return - the data from the api
  */
  char * postEntityStatus( char *entity_id, char *status) {
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
    return doCurl("POST","/entity/status",params);
  }


  /**
   * With a known entity id, a logo object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param logo_name
   *  @return - the data from the api
  */
  char * postEntityLogo( char *entity_id, char *filedata, char *logo_name) {
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
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityLogo( char *entity_id, char *gen_id) {
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
   * With a known entity id, avideo object can be added.
   *
   *  @param entity_id
   *  @param title
   *  @param embed_code
   *  @return - the data from the api
  */
  char * postEntityVideoYoutube( char *entity_id, char *title, char *embed_code) {
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
    strcat(params,"embed_code=");
    p = curl_easy_escape(curl,embed_code,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/video/youtube",params);
  }


  /**
   * Allows a video object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityVideo( char *entity_id, char *gen_id) {
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
   * With a known entity id, an affiliate link object can be added.
   *
   *  @param entity_id
   *  @param affiliate_name
   *  @param affiliate_link
   *  @param affiliate_message
   *  @param affiliate_logo
   *  @return - the data from the api
  */
  char * postEntityAffiliate_link( char *entity_id, char *affiliate_name, char *affiliate_link, char *affiliate_message, char *affiliate_logo) {
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
    return doCurl("POST","/entity/affiliate_link",params);
  }


  /**
   * Allows an affiliate link object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityAffiliate_link( char *entity_id, char *gen_id) {
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
   * With a known entity id, a description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @return - the data from the api
  */
  char * postEntityDescription( char *entity_id, char *headline, char *body) {
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
    return doCurl("POST","/entity/description",params);
  }


  /**
   * Allows a description object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityDescription( char *entity_id, char *gen_id) {
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
   * With a known entity id, a list description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @return - the data from the api
  */
  char * postEntityList( char *entity_id, char *headline, char *body) {
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
   * Allows a list description object to be reduced in confidence
   *
   *  @param gen_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * deleteEntityList( char *gen_id, char *entity_id) {
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
   * With a known entity id, an document object can be added.
   *
   *  @param entity_id
   *  @param name
   *  @param filedata
   *  @return - the data from the api
  */
  char * postEntityDocument( char *entity_id, char *name, char *filedata) {
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
  char * deleteEntityDocument( char *entity_id, char *gen_id) {
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
   * With a known entity id, a testimonial object can be added.
   *
   *  @param entity_id
   *  @param title
   *  @param text
   *  @param date
   *  @param testifier_name
   *  @return - the data from the api
  */
  char * postEntityTestimonial( char *entity_id, char *title, char *text, char *date, char *testifier_name) {
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
   * Allows a testimonial object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityTestimonial( char *entity_id, char *gen_id) {
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
   * With a known entity id, a opening times object can be added. Each day can be either 'closed' to indicate that the entity is closed that day, '24hour' to indicate that the entity is open all day or single/split time ranges can be supplied in 4-digit 24-hour format, such as '09001730' or '09001200,13001700' to indicate hours of opening.
   *
   *  @param entity_id - The id of the entity to edit
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
  char * postEntityOpening_times( char *entity_id, char *monday, char *tuesday, char *wednesday, char *thursday, char *friday, char *saturday, char *sunday, char *closed, char *closed_public_holidays) {
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
   * With a known entity id, a website object can be added.
   *
   *  @param entity_id
   *  @param title
   *  @param description
   *  @param terms
   *  @param start_date
   *  @param expiry_date
   *  @param url
   *  @param image_url
   *  @return - the data from the api
  */
  char * postEntitySpecial_offer( char *entity_id, char *title, char *description, char *terms, char *start_date, char *expiry_date, char *url, char *image_url) {
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
    strcat(params,"image_url=");
    p = curl_easy_escape(curl,image_url,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("POST","/entity/special_offer",params);
  }


  /**
   * Allows a special offer object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntitySpecial_offer( char *entity_id, char *gen_id) {
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
   * Update user based on email address or social_network/social_network_id
   *
   *  @param email
   *  @param first_name
   *  @param last_name
   *  @param active
   *  @param trust
   *  @param creation_date
   *  @param user_type
   *  @param social_network
   *  @param social_network_id
   *  @param reseller_admin_masheryid
   *  @return - the data from the api
  */
  char * postUser( char *email, char *first_name, char *last_name, char *active, char *trust, char *creation_date, char *user_type, char *social_network, char *social_network_id, char *reseller_admin_masheryid) {
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
    strcat(params,"active=");
    p = curl_easy_escape(curl,active,0);
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
    return doCurl("POST","/user",params);
  }


  /**
   * With a unique email address an user can be retrieved
   *
   *  @param email
   *  @return - the data from the api
  */
  char * getUserBy_email( char *email) {
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
   * With a unique ID address an user can be retrieved
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * getUser( char *user_id) {
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
   * With a unique ID address an user can be retrieved
   *
   *  @param name
   *  @param id
   *  @return - the data from the api
  */
  char * getUserBy_social_media( char *name, char *id) {
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
   * Returns all the users that match the supplied reseller_admin_masheryid
   *
   *  @param reseller_admin_masheryid
   *  @return - the data from the api
  */
  char * getUserBy_reseller_admin_masheryid( char *reseller_admin_masheryid) {
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
   * Removes reseller privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * postUserReseller_remove( char *user_id) {
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
   * The search matches a category name on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * getAutocompleteCategory( char *str, char *language) {
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
    return doCurl("GET","/autocomplete/category",params);
  }


  /**
   * The search matches a category name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * getAutocompleteKeyword( char *str, char *language) {
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
   *  @return - the data from the api
  */
  char * getAutocompleteLocation( char *str, char *country) {
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
    return doCurl("GET","/autocomplete/location",params);
  }


  /**
   * The search matches a postcode to the supplied string
   *
   *  @param str - A string to search against, E.g. W1 e.g. W1
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. gb e.g. gb
   *  @return - the data from the api
  */
  char * getAutocompletePostcode( char *str, char *country) {
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
    return doCurl("GET","/autocomplete/postcode",params);
  }


  /**
   * Create a queue item
   *
   *  @param queue_name
   *  @param data
   *  @return - the data from the api
  */
  char * putQueue( char *queue_name, char *data) {
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
  char * deleteQueue( char *queue_id) {
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
   * Retrieve queue items.
   *
   *  @param limit
   *  @param queue_name
   *  @return - the data from the api
  */
  char * getQueue( char *limit, char *queue_name) {
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
   * Unlock queue items.
   *
   *  @param queue_name
   *  @param seconds
   *  @return - the data from the api
  */
  char * postQueueUnlock( char *queue_name, char *seconds) {
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
   * Add an error to a queue item
   *
   *  @param queue_id
   *  @param error
   *  @return - the data from the api
  */
  char * postQueueError( char *queue_id, char *error) {
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
  char * getQueueSearch( char *type, char *id) {
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
  char * putTransaction( char *entity_id, char *user_id, char *basket_total, char *basket, char *currency, char *notes) {
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
   * Set a transactions status to inprogess
   *
   *  @param transaction_id
   *  @param paypal_setexpresscheckout
   *  @return - the data from the api
  */
  char * postTransactionInprogress( char *transaction_id, char *paypal_setexpresscheckout) {
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
   * Set a transactions status to authorised
   *
   *  @param transaction_id
   *  @param paypal_getexpresscheckoutdetails
   *  @return - the data from the api
  */
  char * postTransactionAuthorised( char *transaction_id, char *paypal_getexpresscheckoutdetails) {
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
   * Set a transactions status to complete
   *
   *  @param transaction_id
   *  @param paypal_doexpresscheckoutpayment
   *  @param user_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * postTransactionComplete( char *transaction_id, char *paypal_doexpresscheckoutpayment, char *user_id, char *entity_id) {
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
   * Set a transactions status to cancelled
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * postTransactionCancelled( char *transaction_id) {
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
   * Given a transaction_id retrieve information on it
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * getTransaction( char *transaction_id) {
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
   * Given a transaction_id retrieve information on it
   *
   *  @param paypal_transaction_id
   *  @return - the data from the api
  */
  char * getTransactionBy_paypal_transaction_id( char *paypal_transaction_id) {
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
   * Allow an entity to be claimed by a valid user
   *
   *  @param entity_id
   *  @param claimed_user_id
   *  @param claimed_date
   *  @param claim_method
   *  @param phone_number
   *  @return - the data from the api
  */
  char * postEntityClaim( char *entity_id, char *claimed_user_id, char *claimed_date, char *claim_method, char *phone_number) {
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
    strcat(params,"claimed_date=");
    p = curl_easy_escape(curl,claimed_date,0);
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
    return doCurl("POST","/entity/claim",params);
  }


  /**
   * Update/Add a publisher
   *
   *  @param publisher_id
   *  @param country
   *  @param name
   *  @param description
   *  @param active
   *  @return - the data from the api
  */
  char * postPublisher( char *publisher_id, char *country, char *name, char *description, char *active) {
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
    return doCurl("POST","/publisher",params);
  }


  /**
   * Delete a publisher with a specified publisher_id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * deletePublisher( char *publisher_id) {
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
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * getPublisher( char *publisher_id) {
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
   * Returns publisher that matches a given publisher id
   *
   *  @param country
   *  @return - the data from the api
  */
  char * getPublisherByCountry( char *country) {
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
  char * getPublisherByEntityId( char *entity_id) {
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
   *  @param claimPrice
   *  @param claimMethods
   *  @return - the data from the api
  */
  char * postCountry( char *country_id, char *name, char *synonyms, char *continentName, char *continent, char *geonameId, char *dbpediaURL, char *freebaseURL, char *population, char *currencyCode, char *languages, char *areaInSqKm, char *capital, char *east, char *west, char *north, char *south, char *claimPrice, char *claimMethods) {
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
    strcat(params,"claimPrice=");
    p = curl_easy_escape(curl,claimPrice,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"claimMethods=");
    p = curl_easy_escape(curl,claimMethods,0);
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
  char * getCountry( char *country_id) {
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
   * For insance, reporting a phone number as wrong
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param gen_id - The gen_id for the item being reported
   *  @param signal_type - The signal that is to be reported e.g. wrong
   *  @param data_type - The type of data being reported
   *  @return - the data from the api
  */
  char * postSignal( char *entity_id, char *gen_id, char *signal_type, char *data_type) {
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
    return doCurl("POST","/signal",params);
  }


  /**
   * Get the number of times an entity has been served out as an advert or on serps/bdp pages
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @param month - The month to report on
   *  @return - the data from the api
  */
  char * getStatsEntityBy_date( char *entity_id, char *year, char *month) {
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
   *  @return - the data from the api
  */
  char * postTraction( char *traction_id, char *trigger_type, char *action_type, char *country, char *email_addresses, char *title, char *body, char *api_method, char *api_url, char *api_params, char *active) {
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
    return doCurl("POST","/traction",params);
  }


  /**
   * Fetching a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * getTraction( char *traction_id) {
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
   * Fetching active tractions
   *
   *  @return - the data from the api
  */
  char * getTractionActive() {
    CURL *curl = curl_easy_init();
    char params[10000];
    char *p;
    strcpy(params,"?api_key=");
    strcat(params,API_KEY);
    strcat(params,"?");
    return doCurl("GET","/traction/active",params);
  }


  /**
   * Deleting a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * deleteTraction( char *traction_id) {
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
  char * postMessage( char *message_id, char *ses_id, char *from_user_id, char *from_email, char *to_entity_id, char *to_email, char *subject, char *body, char *bounced) {
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
   * Fetching a message
   *
   *  @param message_id - The message id to pull the message for
   *  @return - the data from the api
  */
  char * getMessage( char *message_id) {
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
   * Fetching messages by ses_id
   *
   *  @param ses_id - The amazon id to pull the message for
   *  @return - the data from the api
  */
  char * getMessageBy_ses_id( char *ses_id) {
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
   * Update/Add a flatpack
   *
   *  @param flatpack_id - this record's unique, auto-generated id - if supplied, then this is an edit, otherwise it's an add
   *  @param domainName - the domain name to serve this flatpack site on (no leading http:// or anything please)
   *  @param flatpackName - the name of the Flat pack instance
   *  @param less - the LESS configuration to use to overrides the Bootstrap CSS
   *  @param language - the language in which to render the flatpack site
   *  @param country - the country to use for searches etc
   *  @param mapsType - the type of maps to use
   *  @param mapKey - the nokia map key to use to render maps
   *  @param analyticsHTML - the html to insert to record page views
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
   *  @param adblockHeader - the html (JS) to render an advert
   *  @param adblock728x90 - the html (JS) to render a 728x90 advert
   *  @param adblock468x60 - the html (JS) to render a 468x60 advert
   *  @param header_menu - the JSON that describes a navigation at the top of the page
   *  @param footer_menu - the JSON that describes a navigation at the bottom of the page
   *  @param bdpTitle - The page title of the entity business profile pages
   *  @param bdpDescription - The meta description of entity business profile pages
   *  @param bdpAds - The block of HTML/JS that renders Ads on BDPs
   *  @param serpTitle - The page title of the serps
   *  @param serpDescription - The meta description of serps
   *  @param serpNumberResults - The number of results per search page
   *  @param serpNumberAdverts - The number of adverts to show on the first search page
   *  @param serpAds - The block of HTML/JS that renders Ads on Serps
   *  @param cookiePolicyUrl - The cookie policy url of the flatpack
   *  @param cookiePolicyNotice - Whether to show the cookie policy on this flatpack
   *  @param addBusinessButtonText - The text used in the 'Add your business' button
   *  @param twitterUrl - Twitter link
   *  @param facebookUrl - Facebook link
   *  @return - the data from the api
  */
  char * postFlatpack( char *flatpack_id, char *domainName, char *flatpackName, char *less, char *language, char *country, char *mapsType, char *mapKey, char *analyticsHTML, char *searchFormShowOn, char *searchFormShowKeywordsBox, char *searchFormShowLocationBox, char *searchFormKeywordsAutoComplete, char *searchFormLocationsAutoComplete, char *searchFormDefaultLocation, char *searchFormPlaceholderKeywords, char *searchFormPlaceholderLocation, char *searchFormKeywordsLabel, char *searchFormLocationLabel, char *cannedLinksHeader, char *homepageTitle, char *homepageDescription, char *homepageIntroTitle, char *homepageIntroText, char *adblockHeader, char *adblock728x90, char *adblock468x60, char *header_menu, char *footer_menu, char *bdpTitle, char *bdpDescription, char *bdpAds, char *serpTitle, char *serpDescription, char *serpNumberResults, char *serpNumberAdverts, char *serpAds, char *cookiePolicyUrl, char *cookiePolicyNotice, char *addBusinessButtonText, char *twitterUrl, char *facebookUrl) {
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
    strcat(params,"analyticsHTML=");
    p = curl_easy_escape(curl,analyticsHTML,0);
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
    strcat(params,"adblockHeader=");
    p = curl_easy_escape(curl,adblockHeader,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adblock728x90=");
    p = curl_easy_escape(curl,adblock728x90,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"adblock468x60=");
    p = curl_easy_escape(curl,adblock468x60,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"header_menu=");
    p = curl_easy_escape(curl,header_menu,0);
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
    return doCurl("POST","/flatpack",params);
  }


  /**
   * Get a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * getFlatpack( char *flatpack_id) {
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
   * Get a flatpack using a domain name
   *
   *  @param domainName - the domain name to search for
   *  @return - the data from the api
  */
  char * getFlatpackBy_domain_name( char *domainName) {
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
    return doCurl("GET","/flatpack/by_domain_name",params);
  }


  /**
   * Remove a flatpack using a supplied flatpack_id
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @return - the data from the api
  */
  char * deleteFlatpack( char *flatpack_id) {
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
   * Add a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param keywords - the keywords to use in the canned search
   *  @param location - the location to use in the canned search
   *  @param linkText - the link text to be used to in the canned search link
   *  @return - the data from the api
  */
  char * postFlatpackLink( char *flatpack_id, char *keywords, char *location, char *linkText) {
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
   * Remove a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param gen_id - the id of the canned link to remove
   *  @return - the data from the api
  */
  char * deleteFlatpackLink( char *flatpack_id, char *gen_id) {
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
   * Upload a logo to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackLogo( char *flatpack_id, char *filedata) {
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
   * Upload a file to our asset server and return the url
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackUpload( char *filedata) {
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
    return doCurl("POST","/flatpack/upload",params);
  }


  /**
   * Upload an icon to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackIcon( char *flatpack_id, char *filedata) {
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
   * Allows us to identify the user, entity and element from an encoded endpoint URL's token
   *
   *  @param token
   *  @return - the data from the api
  */
  char * getTokenDecode( char *token) {
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
   * Provides a tokenised URL to redirect a user so they can add an entity to Central Index
   *
   *  @param language - The language to use to render the add path e.g. en
   *  @param portal_name - The name of the website that data is to be added on e.g. YourLocal
   *  @return - the data from the api
  */
  char * getTokenAdd( char *language, char *portal_name) {
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
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/add",params);
  }


  /**
   * Provides a tokenised URL to redirect a user to claim an entity on Central Index
   *
   *  @param entity_id - Entity ID to be claimed e.g. 380348266819584
   *  @param language - The language to use to render the claim path e.g. en
   *  @param portal_name - The name of the website that entity is being claimed on e.g. YourLocal
   *  @return - the data from the api
  */
  char * getTokenClaim( char *entity_id, char *language, char *portal_name) {
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
    strcat(params,"portal_name=");
    p = curl_easy_escape(curl,portal_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/token/claim",params);
  }


  /**
   * Provides a tokenised URL that allows a user to report incorrect entity information
   *
   *  @param entity_id - The unique Entity ID e.g. 379236608286720
   *  @param portal_name - The name of the portal that the user is coming from e.g. YourLocal
   *  @param language - The language to use to render the report path
   *  @return - the data from the api
  */
  char * getTokenReport( char *entity_id, char *portal_name, char *language) {
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
    return doCurl("GET","/token/report",params);
  }


  /**
   * Fetch token for messaging path
   *
   *  @param entity_id - The id of the entity being messaged
   *  @param portal_name - The name of the application that has initiated the email process, example: 'Your Local'
   *  @param language - The language for the app
   *  @return - the data from the api
  */
  char * getTokenMessage( char *entity_id, char *portal_name, char *language) {
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
    return doCurl("GET","/token/message",params);
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
  char * postEmail( char *to_email_address, char *reply_email_address, char *source_account, char *subject, char *body, char *html_body) {
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
   * Log a sale
   *
   *  @param entity_id - The entity the sale was made against
   *  @param action_type - The type of action we are performing
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
  char * postSales_log( char *entity_id, char *action_type, char *publisher_id, char *mashery_id, char *reseller_ref, char *reseller_agent_id, char *max_tags, char *max_locations, char *extra_tags, char *extra_locations, char *expiry_date) {
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
    strcat(params,"action_type=");
    p = curl_easy_escape(curl,action_type,0);
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
    return doCurl("POST","/sales_log",params);
  }


  /**
   * Return a sales log by id
   *
   *  @param sales_log_id - The sales log id to pull
   *  @return - the data from the api
  */
  char * getSales_log( char *sales_log_id) {
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
   * With a known entity id, a social media object can be added.
   *
   *  @param entity_id
   *  @param type
   *  @param website_url
   *  @return - the data from the api
  */
  char * postEntitySocialmedia( char *entity_id, char *type, char *website_url) {
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
  char * deleteEntitySocialmedia( char *entity_id, char *gen_id) {
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
   * With a known entity id, a private object can be added.
   *
   *  @param entity_id - The entity to associate the private object with
   *  @param data - The data to store
   *  @return - the data from the api
  */
  char * putPrivate_object( char *entity_id, char *data) {
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
   * Allows a private object to be removed
   *
   *  @param private_object_id - The id of the private object to remove
   *  @return - the data from the api
  */
  char * deletePrivate_object( char *private_object_id) {
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
   * Allows a private object to be returned based on the entity_id and masheryid
   *
   *  @param entity_id - The entity associated with the private object
   *  @return - the data from the api
  */
  char * getPrivate_objectAll( char *entity_id) {
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
   * Update/Add a Group
   *
   *  @param group_id
   *  @param name
   *  @param description
   *  @param url
   *  @return - the data from the api
  */
  char * postGroup( char *group_id, char *name, char *description, char *url) {
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
    return doCurl("POST","/group",params);
  }


  /**
   * Delete a group with a specified group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * deleteGroup( char *group_id) {
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
   * Returns group that matches a given group id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * getGroup( char *group_id) {
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
   * With a known entity id, a group  can be added to group members.
   *
   *  @param entity_id
   *  @param group_id
   *  @return - the data from the api
  */
  char * postEntityGroup( char *entity_id, char *group_id) {
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
  char * deleteEntityGroup( char *entity_id, char *gen_id) {
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





