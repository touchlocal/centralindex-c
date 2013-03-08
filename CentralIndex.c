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
   *  @param what
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
   * Supply an entity and an object within it (e.g. a phone number), and retrieve a URL that allows the user to report an issue with that object
   *
   *  @param entity_id - The unique Entity ID e.g. 379236608286720
   *  @param gen_id - A Unique ID for the object you wish to report, E.g. Phone number e.g. 379236608299008
   *  @param language
   *  @return - the data from the api
  */
  char * getEntityReport( char *entity_id, char *gen_id, char *language) {
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
    strcat(params,"language=");
    p = curl_easy_escape(curl,language,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("GET","/entity/report",params);
  }


  /**
   * Allows us to identify the user, entity and element from an encoded endpoint URL's token
   *
   *  @param token
   *  @return - the data from the api
  */
  char * getToolsDecodereport( char *token) {
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
    return doCurl("GET","/tools/decodereport",params);
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
   *  @param telephone_type
   *  @param email
   *  @param website
   *  @param category_id
   *  @param category_name
   *  @return - the data from the api
  */
  char * putBusiness( char *name, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *telephone_type, char *email, char *website, char *category_id, char *category_name) {
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
    strcat(params,"telephone_type=");
    p = curl_easy_escape(curl,telephone_type,0);
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
    strcat(params,"category_name=");
    p = curl_easy_escape(curl,category_name,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    return doCurl("PUT","/business",params);
  }


  /**
   * Provides a personalised URL to redirect a user to add an entity to Central Index
   *
   *  @param language - The language to use to render the add path
   *  @return - the data from the api
  */
  char * getEntityAdd( char *language) {
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
    return doCurl("GET","/entity/add",params);
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
   *  @param premium_rate
   *  @param telephone_type
   *  @param tps
   *  @param ctps
   *  @return - the data from the api
  */
  char * postEntityPhone( char *entity_id, char *number, char *description, char *premium_rate, char *telephone_type, char *tps, char *ctps) {
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
    strcat(params,"premium_rate=");
    p = curl_easy_escape(curl,premium_rate,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"telephone_type=");
    p = curl_easy_escape(curl,telephone_type,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"tps=");
    p = curl_easy_escape(curl,tps,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"ctps=");
    p = curl_easy_escape(curl,ctps,0);
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
   *  @param premium_rate
   *  @return - the data from the api
  */
  char * postEntityFax( char *entity_id, char *number, char *description, char *premium_rate) {
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
    strcat(params,"premium_rate=");
    p = curl_easy_escape(curl,premium_rate,0);
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
   * With a known entity id, an category object can be added.
   *
   *  @param entity_id
   *  @param category_id
   *  @param category_name
   *  @return - the data from the api
  */
  char * postEntityCategory( char *entity_id, char *category_id, char *category_name) {
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
    strcat(params,"category_name=");
    p = curl_easy_escape(curl,category_name,0);
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
   *  @param name_strictness
   *  @param location_strictness
   *  @return - the data from the api
  */
  char * getMatchByphone( char *phone, char *company_name, char *latitude, char *longitude, char *name_strictness, char *location_strictness) {
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
   *  @return - the data from the api
  */
  char * getToolsSpider( char *url) {
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
    return doCurl("GET","/tools/spider",params);
  }


  /**
   * Supply an address to geocode - returns lat/lon and accuracy
   *
   *  @param address
   *  @return - the data from the api
  */
  char * getToolsGeocode( char *address) {
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
   *  @return - the data from the api
  */
  char * postEntityPostal_address( char *entity_id, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *postcode, char *address_type) {
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
    return doCurl("POST","/entity/postal_address",params);
  }


  /**
   * With a known entity id, a advertiser is added
   *
   *  @param entity_id
   *  @param tags
   *  @param locations
   *  @param expiry
   *  @param is_national
   *  @param language
   *  @return - the data from the api
  */
  char * postEntityAdvertiser( char *entity_id, char *tags, char *locations, char *expiry, char *is_national, char *language) {
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
    strcat(params,"expiry=");
    p = curl_easy_escape(curl,expiry,0);
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
    return doCurl("POST","/entity/advertiser",params);
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
   *  @return - the data from the api
  */
  char * postLocation( char *location_id, char *name, char *formal_name, char *latitude, char *longitude, char *resolution, char *country, char *population, char *description, char *timezone, char *is_duplicate, char *is_default) {
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
   *  @param description
   *  @param thumbnail
   *  @param embed_code
   *  @return - the data from the api
  */
  char * postEntityVideo( char *entity_id, char *title, char *description, char *thumbnail, char *embed_code) {
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
    strcat(params,"thumbnail=");
    p = curl_easy_escape(curl,thumbnail,0);
    strcat(params,p);
    curl_free(p);
    strcat(params,"&");
    strcat(params,"embed_code=");
    p = curl_easy_escape(curl,embed_code,0);
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
   *  @return - the data from the api
  */
  char * postUser( char *email, char *first_name, char *last_name, char *active, char *trust, char *creation_date, char *user_type, char *social_network, char *social_network_id) {
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
   * The search matches a category name or synonym on a given string and language.
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
   *  @return - the data from the api
  */
  char * postEntityClaim( char *entity_id, char *claimed_user_id, char *claimed_date) {
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
    return doCurl("POST","/entity/claim",params);
  }





