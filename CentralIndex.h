2022/12/14 12:14:44 STARTUP Redis server: tcp://127.0.0.1:6379
2022/12/14 12:14:44 STARTUP ElasticSearch server: http://172.22.12.49:9200
2022/12/14 12:14:44 STARTUP ES view server: http://172.22.114.129:63300/view/view
2022/12/14 12:14:44 STARTUP CouchDB server: http://wolf_staging:******@172.22.10.201:5984
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
static size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream); 
static size_t curl_put( void *ptr, size_t size, size_t nmemb, void *stream ); 
char * doCurl (char * method, char * path, char * params);
void set_api_key ( char * apikey);

/* 1 = DEBUG ON  0 = DEBUG OFF */

void set_debug_mode (int debug);
  /**
   * With a 192 id get remote entity data
   *
   *  @param oneninetwo_id
   *  @return - the data from the api
  */
  char * GET192Get( char *oneninetwo_id);

 
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
  char * GETActivity_stream( char *type, char *country, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *number_results, char *unique_action);

 
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
  char * POSTActivity_stream( char *entity_id, char *entity_name, char *type, char *country, char *longitude, char *latitude);

 
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
  char * GETAdvertiserBy_reseller_masheryid( char *country, char *reseller_masheryid, char *name_only, char *name_match, char *skip, char *take);

 
  /**
   * Get all advertisers that have been updated from a give date for a given reseller
   *
   *  @param from_date
   *  @param country
   *  @return - the data from the api
  */
  char * GETAdvertiserUpdated( char *from_date, char *country);

 
  /**
   * Get all advertisers that have been updated from a give date for a given publisher
   *
   *  @param publisher_id
   *  @param from_date
   *  @param country
   *  @return - the data from the api
  */
  char * GETAdvertiserUpdatedBy_publisher( char *publisher_id, char *from_date, char *country);

 
  /**
   * Check that the advertiser has a premium inventory
   *
   *  @param type
   *  @param category_id - The category of the advertiser
   *  @param location_id - The location of the advertiser
   *  @param publisher_id - The publisher of the advertiser
   *  @return - the data from the api
  */
  char * GETAdvertisersPremiumInventorycheck( char *type, char *category_id, char *location_id, char *publisher_id);

 
  /**
   * Delete an association
   *
   *  @param association_id
   *  @return - the data from the api
  */
  char * DELETEAssociation( char *association_id);

 
  /**
   * Fetch an association
   *
   *  @param association_id
   *  @return - the data from the api
  */
  char * GETAssociation( char *association_id);

 
  /**
   * Will create a new association or update an existing one
   *
   *  @param association_id
   *  @param association_name
   *  @param association_url
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTAssociation( char *association_id, char *association_name, char *association_url, char *filedata);

 
  /**
   * The search matches a category name on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @param mapped_to_partner - Only return CI categories that have a partner mapping
   *  @return - the data from the api
  */
  char * GETAutocompleteCategory( char *str, char *language, char *mapped_to_partner);

 
  /**
   * The search matches a category name and ID on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @param mapped_to_partner - Only return CI categories that have a partner mapping
   *  @return - the data from the api
  */
  char * GETAutocompleteCategoryId( char *str, char *language, char *mapped_to_partner);

 
  /**
   * The search matches a category name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * GETAutocompleteKeyword( char *str, char *language);

 
  /**
   * The search matches a location name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Dub e.g. dub
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * GETAutocompleteLocation( char *str, char *country, char *language);

 
  /**
   * The search matches a location name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Middle e.g. dub
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param resolution
   *  @return - the data from the api
  */
  char * GETAutocompleteLocationBy_resolution( char *str, char *country, char *resolution);

 
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
  char * PUTBusiness( char *name, char *status, char *building_number, char *branch_name, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *allow_no_address, char *allow_no_phone, char *additional_telephone_number, char *email, char *website, char *payment_types, char *tags, char *category_id, char *category_type, char *featured_message_text, char *featured_message_url, char *do_not_display, char *orderonline, char *delivers, char *referrer_url, char *referrer_name, char *destructive, char *delete_mode, char *master_entity_id, char *no_merge_on_error);

 
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
  char * PUTBusinessJson( char *json, char *country, char *timezone, char *master_entity_id, char *allow_no_address, char *allow_no_phone, char *queue_priority, char *skip_dedup_check);

 
  /**
   * Create entity via JSON
   *
   *  @param entity_id - The entity to add rich data too
   *  @param json - The rich data to add to the entity
   *  @return - the data from the api
  */
  char * POSTBusinessJsonProcess( char *entity_id, char *json);

 
  /**
   * Delete a business tool with a specified tool_id
   *
   *  @param tool_id
   *  @return - the data from the api
  */
  char * DELETEBusiness_tool( char *tool_id);

 
  /**
   * Returns business tool that matches a given tool id
   *
   *  @param tool_id
   *  @return - the data from the api
  */
  char * GETBusiness_tool( char *tool_id);

 
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
  char * POSTBusiness_tool( char *tool_id, char *country, char *headline, char *description, char *link_url, char *active);

 
  /**
   * Returns active business tools for a specific masheryid in a given country
   *
   *  @param country
   *  @param activeonly
   *  @return - the data from the api
  */
  char * GETBusiness_toolBy_masheryid( char *country, char *activeonly);

 
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
  char * POSTBusiness_toolCta( char *tool_id, char *enablecta, char *cta_id, char *slug, char *nomodal, char *type, char *headline, char *textshort, char *link, char *linklabel, char *textlong, char *textoutro, char *bullets, char *masheryids, char *imgurl, char *custombranding, char *customcol, char *custombkg, char *customctacol, char *customctabkg, char *custominfocol, char *custominfobkg);

 
  /**
   * Assigns a Business Tool image
   *
   *  @param tool_id
   *  @param assignimage
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTBusiness_toolImage( char *tool_id, char *assignimage, char *filedata);

 
  /**
   * Assigns a Business Tool image
   *
   *  @param tool_id
   *  @param image_url
   *  @return - the data from the api
  */
  char * POSTBusiness_toolImageBy_url( char *tool_id, char *image_url);

 
  /**
   * With a known cache key get the data from cache
   *
   *  @param cache_key
   *  @param use_compression
   *  @return - the data from the api
  */
  char * GETCache( char *cache_key, char *use_compression);

 
  /**
   * Add some data to the cache with a given expiry
   *
   *  @param cache_key
   *  @param expiry - The cache expiry in seconds
   *  @param data - The data to cache
   *  @param use_compression
   *  @return - the data from the api
  */
  char * POSTCache( char *cache_key, char *expiry, char *data, char *use_compression);

 
  /**
   * Returns the supplied wolf category object by fetching the supplied category_id from our categories object.
   *
   *  @param category_id
   *  @return - the data from the api
  */
  char * GETCategory( char *category_id);

 
  /**
   * With a known category id, an category object can be added.
   *
   *  @param category_id
   *  @param language
   *  @param name
   *  @return - the data from the api
  */
  char * PUTCategory( char *category_id, char *language, char *name);

 
  /**
   * Returns all Central Index categories and associated data
   *
   *  @param partner
   *  @return - the data from the api
  */
  char * GETCategoryAll( char *partner);

 
  /**
   * With a known category id, a mapping object can be added.
   *
   *  @param category_id
   *  @param type
   *  @param id
   *  @param name
   *  @return - the data from the api
  */
  char * POSTCategoryMappings( char *category_id, char *type, char *id, char *name);

 
  /**
   * With a known category id, a mapping object can be deleted.
   *
   *  @param category_id
   *  @param category_type
   *  @param mapped_id
   *  @return - the data from the api
  */
  char * DELETECategoryMappings( char *category_id, char *category_type, char *mapped_id);

 
  /**
   * Allows a category object to merged with another
   *
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * POSTCategoryMerge( char *from, char *to);

 
  /**
   * With a known category id, an synonym object can be added.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * POSTCategorySynonym( char *category_id, char *synonym, char *language);

 
  /**
   * With a known category id, a synonyms object can be removed.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * DELETECategorySynonym( char *category_id, char *synonym, char *language);

 
  /**
   * Get the contract from the ID supplied
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * GETContract( char *contract_id);

 
  /**
   * Get the active contracts from the ID supplied
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETContractBy_entity_id( char *entity_id);

 
  /**
   * Get a contract from the payment provider id supplied
   *
   *  @param payment_provider
   *  @param payment_provider_id
   *  @return - the data from the api
  */
  char * GETContractBy_payment_provider_id( char *payment_provider, char *payment_provider_id);

 
  /**
   * Get the active contracts from the ID supplied
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETContractBy_user_id( char *user_id);

 
  /**
   * Cancels an existing contract for a given id
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * POSTContractCancel( char *contract_id);

 
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
  char * POSTContractCreate( char *entity_id, char *user_id, char *payment_provider, char *basket, char *taxrate, char *billing_period, char *source, char *channel, char *campaign, char *referrer_domain, char *referrer_name, char *flatpack_id);

 
  /**
   * Activate a contract that is free
   *
   *  @param contract_id
   *  @param user_name
   *  @param user_surname
   *  @param user_email_address
   *  @return - the data from the api
  */
  char * POSTContractFreeactivate( char *contract_id, char *user_name, char *user_surname, char *user_email_address);

 
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
  char * POSTContractPaymentFailure( char *contract_id, char *failure_reason, char *payment_date, char *amount, char *currency, char *response);

 
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
  char * POSTContractPaymentSetup( char *contract_id, char *payment_provider_id, char *payment_provider_profile, char *user_name, char *user_surname, char *user_billing_address, char *user_email_address);

 
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
  char * POSTContractPaymentSuccess( char *contract_id, char *payment_date, char *amount, char *currency, char *response);

 
  /**
   * Go through all the products in a contract and provision them
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * POSTContractProvision( char *contract_id);

 
  /**
   * Ensures contract has been cancelled for a given id, expected to be called from stripe on deletion of subscription
   *
   *  @param contract_id
   *  @return - the data from the api
  */
  char * POSTContractSubscriptionended( char *contract_id);

 
  /**
   * Get the contract log from the ID supplied
   *
   *  @param contract_log_id
   *  @return - the data from the api
  */
  char * GETContract_log( char *contract_log_id);

 
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
  char * POSTContract_log( char *contract_id, char *date, char *payment_provider, char *response, char *success, char *amount, char *currency);

 
  /**
   * Get the contract logs from the ID supplied
   *
   *  @param contract_id
   *  @param page
   *  @param per_page
   *  @return - the data from the api
  */
  char * GETContract_logBy_contract_id( char *contract_id, char *page, char *per_page);

 
  /**
   * Get the contract logs from the payment_provider supplied
   *
   *  @param payment_provider
   *  @param page
   *  @param per_page
   *  @return - the data from the api
  */
  char * GETContract_logBy_payment_provider( char *payment_provider, char *page, char *per_page);

 
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
  char * POSTCountry( char *country_id, char *name, char *synonyms, char *continentName, char *continent, char *geonameId, char *dbpediaURL, char *freebaseURL, char *population, char *currencyCode, char *languages, char *areaInSqKm, char *capital, char *east, char *west, char *north, char *south, char *claimProductId, char *claimMethods, char *twilio_sms, char *twilio_phone, char *twilio_voice, char *currency_symbol, char *currency_symbol_html, char *postcodeLookupActive, char *addressFields, char *addressMatching, char *dateFormat, char *iso_3166_alpha_3, char *iso_3166_numeric);

 
  /**
   * Fetching a country
   *
   *  @param country_id
   *  @return - the data from the api
  */
  char * GETCountry( char *country_id);

 
  /**
   * An API call to fetch a crash report by its ID
   *
   *  @param crash_report_id - The crash report to pull
   *  @return - the data from the api
  */
  char * GETCrash_report( char *crash_report_id);

 
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
  char * POSTEmail( char *to_email_address, char *reply_email_address, char *source_account, char *subject, char *body, char *html_body);

 
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
  char * PUTEntity( char *type, char *scope, char *country, char *timezone, char *trust, char *our_data);

 
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
  char * GETEntity( char *entity_id, char *domain, char *path, char *data_filter, char *filter_by_confidence);

 
  /**
   * With a known entity id, an add can be updated.
   *
   *  @param entity_id
   *  @param add_referrer_url
   *  @param add_referrer_name
   *  @return - the data from the api
  */
  char * POSTEntityAdd( char *entity_id, char *add_referrer_url, char *add_referrer_name);

 
  /**
   * Allows an advertiser object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAdvertiser( char *entity_id, char *gen_id);

 
  /**
   * Expires an advertiser from and entity
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserCancel( char *entity_id, char *publisher_id, char *reseller_ref, char *reseller_agent_id);

 
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
  char * POSTEntityAdvertiserCreate( char *entity_id, char *tags, char *locations, char *loc_tags, char *region_tags, char *max_tags, char *max_locations, char *expiry_date, char *is_national, char *is_regional, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id);

 
  /**
   * Adds/removes locations
   *
   *  @param entity_id
   *  @param gen_id
   *  @param locations_to_add
   *  @param locations_to_remove
   *  @return - the data from the api
  */
  char * POSTEntityAdvertiserLocation( char *entity_id, char *gen_id, char *locations_to_add, char *locations_to_remove);

 
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
  char * POSTEntityAdvertiserPremiumCancel( char *entity_id, char *publisher_id, char *type, char *category_id, char *location_id);

 
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
  char * POSTEntityAdvertiserPremiumCreate( char *entity_id, char *type, char *category_id, char *location_id, char *expiry_date, char *reseller_ref, char *reseller_agent_id, char *publisher_id);

 
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
  char * POSTEntityAdvertiserPremiumRenew( char *entity_id, char *type, char *category_id, char *location_id, char *expiry_date, char *reseller_ref, char *reseller_agent_id, char *publisher_id);

 
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
  char * POSTEntityAdvertiserRenew( char *entity_id, char *expiry_date, char *publisher_id, char *reseller_ref, char *reseller_agent_id);

 
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
  char * POSTEntityAdvertiserTag( char *gen_id, char *entity_id, char *language, char *tags_to_add, char *tags_to_remove);

 
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
  char * POSTEntityAdvertiserUpsell( char *entity_id, char *tags, char *locations, char *loc_tags, char *is_regional, char *region_tags, char *extra_tags, char *extra_locations, char *is_national, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id);

 
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
  char * GETEntityAdvertisers( char *tag, char *where, char *orderonline, char *delivers, char *isClaimed, char *is_national, char *limit, char *country, char *language, char *benchmark);

 
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
  char * GETEntityAdvertisersBy_location( char *location, char *is_national, char *limit, char *country, char *language);

 
  /**
   * Check if an entity has an advert from a specified publisher
   *
   *  @param entity_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * GETEntityAdvertisersInventorycheck( char *entity_id, char *publisher_id);

 
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
  char * GETEntityAdvertisersPremium( char *what, char *where, char *type, char *country, char *language);

 
  /**
   * Deleteing an affiliate adblock from a known entity
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAffiliate_adblock( char *entity_id, char *gen_id);

 
  /**
   * Adding an affiliate adblock to a known entity
   *
   *  @param entity_id
   *  @param adblock - Number of results returned per page
   *  @return - the data from the api
  */
  char * POSTEntityAffiliate_adblock( char *entity_id, char *adblock);

 
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
  char * POSTEntityAffiliate_link( char *entity_id, char *affiliate_name, char *affiliate_link, char *affiliate_message, char *affiliate_logo, char *affiliate_action);

 
  /**
   * Allows an affiliate link object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAffiliate_link( char *entity_id, char *gen_id);

 
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
  char * POSTEntityAnnouncement( char *entity_id, char *announcement_id, char *headline, char *body, char *link_label, char *link, char *terms_link, char *publish_date, char *expiry_date, char *media_type, char *image_url, char *video_url, char *type);

 
  /**
   * Fetch an announcement object from an existing entity.
   *
   *  @param entity_id
   *  @param announcement_id
   *  @return - the data from the api
  */
  char * GETEntityAnnouncement( char *entity_id, char *announcement_id);

 
  /**
   * Remove an announcement object to an existing entity.
   *
   *  @param entity_id
   *  @param announcement_id
   *  @return - the data from the api
  */
  char * DELETEEntityAnnouncement( char *entity_id, char *announcement_id);

 
  /**
   * Will create a new association_membership or update an existing one
   *
   *  @param entity_id
   *  @param association_id
   *  @param association_member_url
   *  @param association_member_id
   *  @return - the data from the api
  */
  char * POSTEntityAssociation_membership( char *entity_id, char *association_id, char *association_member_url, char *association_member_id);

 
  /**
   * Allows a association_membership object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityAssociation_membership( char *entity_id, char *gen_id);

 
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
  char * POSTEntityBackground( char *entity_id, char *number_of_employees, char *turnover, char *net_profit, char *vat_number, char *duns_number, char *registered_company_number);

 
  /**
   * With a known entity id, a brand object can be added.
   *
   *  @param entity_id
   *  @param value
   *  @return - the data from the api
  */
  char * POSTEntityBrand( char *entity_id, char *value);

 
  /**
   * With a known entity id, a brand object can be deleted.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityBrand( char *entity_id, char *gen_id);

 
  /**
   * Uploads a CSV file of known format and bulk inserts into DB
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTEntityBulkCsv( char *filedata);

 
  /**
   * Shows the current status of a bulk upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * GETEntityBulkCsvStatus( char *upload_id);

 
  /**
   * Uploads a JSON file of known format and bulk inserts into DB
   *
   *  @param data
   *  @param new_entities
   *  @return - the data from the api
  */
  char * POSTEntityBulkJson( char *data, char *new_entities);

 
  /**
   * Shows the current status of a bulk JSON upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * GETEntityBulkJsonStatus( char *upload_id);

 
  /**
   * Fetches the document that matches the given data_source_type and external_id.
   *
   *  @param data_source_type - The data source type of the entity
   *  @param external_id - The external ID of the entity
   *  @return - the data from the api
  */
  char * GETEntityBy_external_id( char *data_source_type, char *external_id);

 
  /**
   * Get all entities within a specified group
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * GETEntityBy_groupid( char *group_id);

 
  /**
   * Fetches the document that matches the given legacy_url
   *
   *  @param legacy_url - The URL of the entity in the directory it was imported from.
   *  @return - the data from the api
  */
  char * GETEntityBy_legacy_url( char *legacy_url);

 
  /**
   * uncontributes a given entities supplier content and makes the entity inactive if the entity is un-usable
   *
   *  @param entity_id - The entity to pull
   *  @param supplier_masheryid - The suppliers masheryid to match
   *  @param supplier_id - The supplier id to match
   *  @param supplier_user_id - The user id to match
   *  @return - the data from the api
  */
  char * DELETEEntityBy_supplier( char *entity_id, char *supplier_masheryid, char *supplier_id, char *supplier_user_id);

 
  /**
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID, or a list of supplier IDs separated by comma
   *  @return - the data from the api
  */
  char * GETEntityBy_supplier_id( char *supplier_id);

 
  /**
   * Get all entities added or claimed by a specific user
   *
   *  @param user_id - The unique user ID of the user with claimed entities e.g. 379236608286720
   *  @param filter
   *  @param skip
   *  @param limit
   *  @return - the data from the api
  */
  char * GETEntityBy_user_id( char *user_id, char *filter, char *skip, char *limit);

 
  /**
   * With a known entity id, an category object can be added.
   *
   *  @param entity_id
   *  @param category_id
   *  @param category_type
   *  @return - the data from the api
  */
  char * POSTEntityCategory( char *entity_id, char *category_id, char *category_type);

 
  /**
   * Allows a category object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityCategory( char *entity_id, char *gen_id);

 
  /**
   * Fetches the changelog documents that match the given entity_id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETEntityChangelog( char *entity_id);

 
  /**
   * Unlike cancel, this operation remove the claim data from the entity
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityClaim( char *entity_id);

 
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
  char * POSTEntityClaim( char *entity_id, char *claimed_user_id, char *claimed_reseller_id, char *expiry_date, char *claimed_date, char *verified_status, char *claim_method, char *phone_number, char *referrer_url, char *referrer_name, char *reseller_ref, char *reseller_description);

 
  /**
   * Cancel a claim that is on the entity
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTEntityClaimCancel( char *entity_id);

 
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
  char * POSTEntityClaimRenew( char *entity_id, char *claimed_user_id, char *reseller_ref, char *reseller_description, char *expiry_date, char *renew_verify);

 
  /**
   * Allow an entity to be claimed by a valid reseller
   *
   *  @param entity_id
   *  @param reseller_ref
   *  @param reseller_description
   *  @return - the data from the api
  */
  char * POSTEntityClaimReseller( char *entity_id, char *reseller_ref, char *reseller_description);

 
  /**
   * If an entity is currently claimed then set or remove the verified_entity block (Expiry will match the claim expiry)
   *
   *  @param entity_id
   *  @param verified_status - If set to a value, this field will promote the claim to pro mode. If blank, verified status will be wiped
   *  @return - the data from the api
  */
  char * POSTEntityClaimVerfied_status( char *entity_id, char *verified_status);

 
  /**
   * Add/change delivers flag for an existing entity - to indicate whether business offers delivery
   *
   *  @param entity_id
   *  @param delivers
   *  @return - the data from the api
  */
  char * POSTEntityDelivers( char *entity_id, char *delivers);

 
  /**
   * Allows a description object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityDescription( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @param gen_id
   *  @return - the data from the api
  */
  char * POSTEntityDescription( char *entity_id, char *headline, char *body, char *gen_id);

 
  /**
   * With a known entity id, an document object can be added.
   *
   *  @param entity_id
   *  @param name
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTEntityDocument( char *entity_id, char *name, char *filedata);

 
  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityDocument( char *entity_id, char *gen_id);

 
  /**
   * Upload a document to an entity
   *
   *  @param entity_id
   *  @param document
   *  @return - the data from the api
  */
  char * POSTEntityDocumentBy_url( char *entity_id, char *document);

 
  /**
   * Allows a email object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityEmail( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, an email address object can be added.
   *
   *  @param entity_id
   *  @param email_address
   *  @param email_description
   *  @return - the data from the api
  */
  char * POSTEntityEmail( char *entity_id, char *email_address, char *email_description);

 
  /**
   * Fetch an emergency statement object from an existing entity.
   *
   *  @param entity_id
   *  @param emergencystatement_id
   *  @return - the data from the api
  */
  char * GETEntityEmergencystatement( char *entity_id, char *emergencystatement_id);

 
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
  char * POSTEntityEmergencystatement( char *entity_id, char *id, char *headline, char *body, char *link_label, char *link, char *publish_date);

 
  /**
   * Remove an emergencystatement object to an existing entity.
   *
   *  @param entity_id
   *  @param emergencystatement_id
   *  @return - the data from the api
  */
  char * DELETEEntityEmergencystatement( char *entity_id, char *emergencystatement_id);

 
  /**
   * Fetch emergency statement objects from an existing entity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETEntityEmergencystatements( char *entity_id);

 
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
  char * POSTEntityEmployee( char *entity_id, char *title, char *forename, char *surname, char *job_title, char *description, char *email, char *phone_number);

 
  /**
   * Allows an employee object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityEmployee( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, an FAQ question and answer can be added.
   *
   *  @param entity_id
   *  @param question
   *  @param answer
   *  @param gen_id
   *  @return - the data from the api
  */
  char * POSTEntityFaq( char *entity_id, char *question, char *answer, char *gen_id);

 
  /**
   * With a known entity id, an FAQ question and answer can be removed.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityFaq( char *entity_id, char *gen_id);

 
  /**
   * Allows a fax object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityFax( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, an fax object can be added.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @return - the data from the api
  */
  char * POSTEntityFax( char *entity_id, char *number, char *description);

 
  /**
   * With a known entity id, a featured message can be added
   *
   *  @param entity_id
   *  @param featured_text
   *  @param featured_url
   *  @return - the data from the api
  */
  char * POSTEntityFeatured_message( char *entity_id, char *featured_text, char *featured_url);

 
  /**
   * Allows a featured message object to be removed
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityFeatured_message( char *entity_id);

 
  /**
   * With a known entity id, a geopoint can be updated.
   *
   *  @param entity_id
   *  @param longitude
   *  @param latitude
   *  @param accuracy
   *  @return - the data from the api
  */
  char * POSTEntityGeopoint( char *entity_id, char *longitude, char *latitude, char *accuracy);

 
  /**
   * With a known entity id, a group  can be added to group members.
   *
   *  @param entity_id
   *  @param group_id
   *  @return - the data from the api
  */
  char * POSTEntityGroup( char *entity_id, char *group_id);

 
  /**
   * Allows a group object to be removed from an entities group members
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityGroup( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a image object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param image_name
   *  @return - the data from the api
  */
  char * POSTEntityImage( char *entity_id, char *filedata, char *image_name);

 
  /**
   * Allows a image object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityImage( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a image can be retrieved from a url and added.
   *
   *  @param entity_id
   *  @param image_url
   *  @param image_name
   *  @return - the data from the api
  */
  char * POSTEntityImageBy_url( char *entity_id, char *image_url, char *image_name);

 
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
  char * POSTEntityInvoice_address( char *entity_id, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *address_type);

 
  /**
   * With a known entity id and a known invoice_address ID, we can delete a specific invoice_address object from an enitity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityInvoice_address( char *entity_id);

 
  /**
   * With a known entity id, a language object can be deleted.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityLanguage( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a language object can be added.
   *
   *  @param entity_id
   *  @param value
   *  @return - the data from the api
  */
  char * POSTEntityLanguage( char *entity_id, char *value);

 
  /**
   * Allows a list description object to be reduced in confidence
   *
   *  @param gen_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityList( char *gen_id, char *entity_id);

 
  /**
   * With a known entity id, a list description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @return - the data from the api
  */
  char * POSTEntityList( char *entity_id, char *headline, char *body);

 
  /**
   * Find all entities in a group
   *
   *  @param group_id - A valid group_id
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @return - the data from the api
  */
  char * GETEntityList_by_group_id( char *group_id, char *per_page, char *page);

 
  /**
   * Adds/removes loc_tags
   *
   *  @param entity_id
   *  @param gen_id
   *  @param loc_tags_to_add
   *  @param loc_tags_to_remove
   *  @return - the data from the api
  */
  char * POSTEntityLoc_tag( char *entity_id, char *gen_id, char *loc_tags_to_add, char *loc_tags_to_remove);

 
  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityLogo( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a logo object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param logo_name
   *  @return - the data from the api
  */
  char * POSTEntityLogo( char *entity_id, char *filedata, char *logo_name);

 
  /**
   * With a known entity id, a logo can be retrieved from a url and added.
   *
   *  @param entity_id
   *  @param logo_url
   *  @param logo_name
   *  @return - the data from the api
  */
  char * POSTEntityLogoBy_url( char *entity_id, char *logo_url, char *logo_name);

 
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
  char * POSTEntityMerge( char *from, char *to, char *override_trust, char *uncontribute_masheryid, char *uncontribute_userid, char *uncontribute_supplierid, char *delete_mode);

 
  /**
   * Update entities that use an old category ID to a new one
   *
   *  @param from
   *  @param to
   *  @param limit
   *  @return - the data from the api
  */
  char * POSTEntityMigrate_category( char *from, char *to, char *limit);

 
  /**
   * With a known entity id, a name can be updated.
   *
   *  @param entity_id
   *  @param name
   *  @param formal_name
   *  @param branch_name
   *  @return - the data from the api
  */
  char * POSTEntityName( char *entity_id, char *name, char *formal_name, char *branch_name);

 
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
  char * POSTEntityOpening_times( char *entity_id, char *statement, char *monday, char *tuesday, char *wednesday, char *thursday, char *friday, char *saturday, char *sunday, char *closed, char *closed_public_holidays);

 
  /**
   * With a known entity id, a opening times object can be removed.
   *
   *  @param entity_id - The id of the entity to edit
   *  @return - the data from the api
  */
  char * DELETEEntityOpening_times( char *entity_id);

 
  /**
   * Add an order online to an existing entity - to indicate e-commerce capability.
   *
   *  @param entity_id
   *  @param orderonline
   *  @return - the data from the api
  */
  char * POSTEntityOrderonline( char *entity_id, char *orderonline);

 
  /**
   * Allows a payment_type object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityPayment_type( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a payment_type object can be added.
   *
   *  @param entity_id - the id of the entity to add the payment type to
   *  @param payment_type - the payment type to add to the entity
   *  @return - the data from the api
  */
  char * POSTEntityPayment_type( char *entity_id, char *payment_type);

 
  /**
   * Allows a new phone object to be added to a specified entity. A new object id will be calculated and returned to you if successful.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @param trackable
   *  @return - the data from the api
  */
  char * POSTEntityPhone( char *entity_id, char *number, char *description, char *trackable);

 
  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityPhone( char *entity_id, char *gen_id);

 
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
  char * POSTEntityPostal_address( char *entity_id, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *address_type, char *do_not_display);

 
  /**
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID
   *  @return - the data from the api
  */
  char * GETEntityProvisionalBy_supplier_id( char *supplier_id);

 
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
  char * POSTEntityPurge( char *entity_id, char *purge_masheryid, char *purge_supplier_id, char *purge_user_id, char *exclude, char *destructive);

 
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
  char * POSTEntityPurgeBy_object( char *entity_id, char *object, char *gen_id, char *purge_masheryid, char *purge_supplier_id, char *purge_user_id, char *destructive);

 
  /**
   * Deletes a specific review for an entity via Review API
   *
   *  @param entity_id - The entity with the review
   *  @param review_id - The review id
   *  @return - the data from the api
  */
  char * DELETEEntityReview( char *entity_id, char *review_id);

 
  /**
   * Gets a specific review  for an entity
   *
   *  @param entity_id - The entity with the review
   *  @param review_id - The review id
   *  @return - the data from the api
  */
  char * GETEntityReview( char *entity_id, char *review_id);

 
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
  char * POSTEntityReview( char *entity_id, char *reviewer_user_id, char *review_id, char *title, char *content, char *star_rating, char *domain, char *filedata);

 
  /**
   * Gets all reviews for an entity
   *
   *  @param entity_id - The entity with the review
   *  @param limit - Limit the number of results returned
   *  @param skip - Number of results skipped
   *  @return - the data from the api
  */
  char * GETEntityReviewList( char *entity_id, char *limit, char *skip);

 
  /**
   * Allows a list of available revisions to be returned by its entity id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETEntityRevisions( char *entity_id);

 
  /**
   * Allows a specific revision of an entity to be returned by entity id and a revision number
   *
   *  @param entity_id
   *  @param revision_id
   *  @return - the data from the api
  */
  char * GETEntityRevisionsByRevisionID( char *entity_id, char *revision_id);

 
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
  char * GETEntitySearchByboundingbox( char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids);

 
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
  char * GETEntitySearchBylocation( char *where, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids, char *benchmark);

 
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
  char * GETEntitySearchGroupBynearest( char *group_id, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *where, char *domain, char *path, char *unitOfDistance, char *restrict_category_ids);

 
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
  char * GETEntitySearchKeywordBynearest( char *keyword, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids, char *include_ads);

 
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
  char * GETEntitySearchWhat( char *what, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids, char *benchmark);

 
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
  char * GETEntitySearchWhatByboundingbox( char *what, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids);

 
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
  char * GETEntitySearchWhatBylocation( char *what, char *where, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids, char *benchmark);

 
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
  char * GETEntitySearchWhatBynearest( char *what, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids);

 
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
  char * GETEntitySearchWho( char *who, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids, char *benchmark);

 
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
  char * GETEntitySearchWhoByboundingbox( char *who, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *language, char *domain, char *path, char *restrict_category_ids);

 
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
  char * GETEntitySearchWhoBylocation( char *who, char *where, char *orderonline, char *delivers, char *isClaimed, char *per_page, char *page, char *country, char *latitude, char *longitude, char *language, char *domain, char *path, char *restrict_category_ids, char *benchmark);

 
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
  char * GETEntitySearchWhoBynearest( char *who, char *orderonline, char *delivers, char *isClaimed, char *country, char *per_page, char *page, char *language, char *latitude, char *longitude, char *domain, char *path, char *restrict_category_ids);

 
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
  char * POSTEntitySend_email( char *entity_id, char *gen_id, char *from_email, char *subject, char *content);

 
  /**
   * With a known entity id, a service object can be added.
   *
   *  @param entity_id
   *  @param value
   *  @return - the data from the api
  */
  char * POSTEntityService( char *entity_id, char *value);

 
  /**
   * With a known entity id, a service object can be deleted.
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityService( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a social media object can be added.
   *
   *  @param entity_id
   *  @param type
   *  @param website_url
   *  @return - the data from the api
  */
  char * POSTEntitySocialmedia( char *entity_id, char *type, char *website_url);

 
  /**
   * Allows a social media object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntitySocialmedia( char *entity_id, char *gen_id);

 
  /**
   * Allows a special offer object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntitySpecial_offer( char *entity_id, char *gen_id);

 
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
  char * POSTEntitySpecial_offer( char *entity_id, char *title, char *description, char *terms, char *start_date, char *expiry_date, char *url);

 
  /**
   * With a known entity id, a status object can be updated.
   *
   *  @param entity_id
   *  @param status
   *  @param inactive_reason
   *  @param inactive_description
   *  @return - the data from the api
  */
  char * POSTEntityStatus( char *entity_id, char *status, char *inactive_reason, char *inactive_description);

 
  /**
   * Suspend all entiies added or claimed by a specific user
   *
   *  @param user_id - The unique user ID of the user with claimed entities e.g. 379236608286720
   *  @return - the data from the api
  */
  char * POSTEntityStatusSuspend_by_user_id( char *user_id);

 
  /**
   * Allows a tag object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityTag( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, an tag object can be added.
   *
   *  @param entity_id
   *  @param tag
   *  @param language
   *  @return - the data from the api
  */
  char * POSTEntityTag( char *entity_id, char *tag, char *language);

 
  /**
   * Allows a testimonial object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityTestimonial( char *entity_id, char *gen_id);

 
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
  char * POSTEntityTestimonial( char *entity_id, char *title, char *text, char *date, char *testifier_name);

 
  /**
   * Get the updates a uncontribute would perform
   *
   *  @param entity_id - The entity to pull
   *  @param object_name - The entity object to update
   *  @param supplier_id - The supplier_id to remove
   *  @param user_id - The user_id to remove
   *  @return - the data from the api
  */
  char * GETEntityUncontribute( char *entity_id, char *object_name, char *supplier_id, char *user_id);

 
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
  char * POSTEntityUnmerge( char *entity_id, char *unmerge_masheryid, char *unmerge_supplier_id, char *unmerge_user_id, char *destructive);

 
  /**
   * Find the provided user in all the sub objects and update the trust
   *
   *  @param entity_id - the entity_id to update
   *  @param user_id - the user to search for
   *  @param trust - The new trust for the user
   *  @return - the data from the api
  */
  char * POSTEntityUser_trust( char *entity_id, char *user_id, char *trust);

 
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
  char * POSTEntityVerified( char *entity_id, char *public_source, char *source_name, char *source_id, char *source_url, char *source_logo, char *verified_date);

 
  /**
   * Remove a verified source object to an existing entity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEEntityVerified( char *entity_id);

 
  /**
   * With a known entity id, a video object can be added.
   *
   *  @param entity_id
   *  @param type
   *  @param link
   *  @return - the data from the api
  */
  char * POSTEntityVideo( char *entity_id, char *type, char *link);

 
  /**
   * Allows a video object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityVideo( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a YouTube video object can be added.
   *
   *  @param entity_id
   *  @param embed_code
   *  @return - the data from the api
  */
  char * POSTEntityVideoYoutube( char *entity_id, char *embed_code);

 
  /**
   * Allows a website object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @param force
   *  @return - the data from the api
  */
  char * DELETEEntityWebsite( char *entity_id, char *gen_id, char *force);

 
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
  char * POSTEntityWebsite( char *entity_id, char *website_url, char *display_url, char *website_description, char *gen_id);

 
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
  char * POSTEntityYext_list( char *entity_id, char *yext_list_id, char *description, char *name, char *type);

 
  /**
   * Allows a yext list object to be removed
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEEntityYext_list( char *entity_id, char *gen_id);

 
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
  char * PUTEntityserve( char *entity_id, char *country, char *event_type, char *domain, char *path);

 
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
  char * POSTFlatpack( char *flatpack_id, char *status, char *nodefaults, char *domainName, char *inherits, char *stub, char *flatpackName, char *less, char *language, char *country, char *mapsType, char *mapKey, char *searchFormShowOn, char *searchFormShowKeywordsBox, char *searchFormShowLocationBox, char *searchFormKeywordsAutoComplete, char *searchFormLocationsAutoComplete, char *searchFormDefaultLocation, char *searchFormPlaceholderKeywords, char *searchFormPlaceholderLocation, char *searchFormKeywordsLabel, char *searchFormLocationLabel, char *cannedLinksHeader, char *homepageTitle, char *homepageDescription, char *homepageIntroTitle, char *homepageIntroText, char *head, char *adblock, char *bodyTop, char *bodyBottom, char *header_menu, char *header_menu_bottom, char *footer_menu, char *bdpTitle, char *bdpDescription, char *bdpAds, char *serpTitle, char *serpDescription, char *serpNumberResults, char *serpNumberAdverts, char *serpAds, char *serpAdsBottom, char *serpTitleNoWhat, char *serpDescriptionNoWhat, char *cookiePolicyUrl, char *cookiePolicyNotice, char *addBusinessButtonText, char *twitterUrl, char *facebookUrl, char *copyright, char *phoneReveal, char *loginLinkText, char *contextLocationId, char *addBusinessButtonPosition, char *denyIndexing, char *contextRadius, char *activityStream, char *activityStreamSize, char *products, char *linkToRoot, char *termsLink, char *serpNumberEmbedAdverts, char *serpEmbedTitle, char *adminLess, char *adminConfNoLocz, char *adminConfNoSocialLogin, char *adminConfEasyClaim, char *adminConfPaymentMode, char *adminConfEnableProducts, char *adminConfSimpleadmin);

 
  /**
   * Get a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETFlatpack( char *flatpack_id);

 
  /**
   * Remove a flatpack using a supplied flatpack_id
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @return - the data from the api
  */
  char * DELETEFlatpack( char *flatpack_id);

 
  /**
   * Upload a CSS file for the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminCSS( char *flatpack_id, char *filedata);

 
  /**
   * Add a HD Admin logo to a flatpack domain
   *
   *  @param flatpack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminHDLogo( char *flatpack_id, char *filedata);

 
  /**
   * Upload an image to serve out as the large logo in the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminLargeLogo( char *flatpack_id, char *filedata);

 
  /**
   * Upload an image to serve out as the small logo in the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackAdminSmallLogo( char *flatpack_id, char *filedata);

 
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
  char * DELETEFlatpackAds( char *flatpack_id, char *adblock, char *serpAds, char *serpAdsBottom, char *bdpAds);

 
  /**
   * Generate flatpacks based on the files passed in
   *
   *  @param json - The flatpack JSON to make replacements on
   *  @param filedata - a file that contains the replacements in the JSON
   *  @param slack_user
   *  @return - the data from the api
  */
  char * POSTFlatpackBulkJson( char *json, char *filedata, char *slack_user);

 
  /**
   * Get flatpacks by country and location
   *
   *  @param country
   *  @param latitude
   *  @param longitude
   *  @return - the data from the api
  */
  char * GETFlatpackBy_country( char *country, char *latitude, char *longitude);

 
  /**
   * Get flatpacks by country in KML format
   *
   *  @param country
   *  @return - the data from the api
  */
  char * GETFlatpackBy_countryKml( char *country);

 
  /**
   * Get a flatpack using a domain name
   *
   *  @param domainName - the domain name to search for
   *  @param matchAlias - Whether to match alias as well
   *  @return - the data from the api
  */
  char * GETFlatpackBy_domain_name( char *domainName, char *matchAlias);

 
  /**
   * Get flatpacks that match the supplied masheryid
   *
   *  @return - the data from the api
  */
  char * GETFlatpackBy_masheryid();

 
  /**
   * Clone an existing flatpack
   *
   *  @param flatpack_id - the flatpack_id to clone
   *  @param domainName - the domain of the new flatpack site (no leading http:// or anything please)
   *  @return - the data from the api
  */
  char * GETFlatpackClone( char *flatpack_id, char *domainName);

 
  /**
   * undefined
   *
   *  @param flatpack_id - the unique id to search for
   *  @param domainName
   *  @return - the data from the api
  */
  char * POSTFlatpackDomain_alias( char *flatpack_id, char *domainName);

 
  /**
   * undefined
   *
   *  @param flatpack_id - the unique id to search for
   *  @param domainName
   *  @return - the data from the api
  */
  char * DELETEFlatpackDomain_alias( char *flatpack_id, char *domainName);

 
  /**
   * Returns a list of domain names in which direct/inherited flatpack country match the specified one and status equals production.
   *
   *  @param country
   *  @return - the data from the api
  */
  char * GETFlatpackDomain_nameBy_country( char *country);

 
  /**
   * Upload an icon to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackIcon( char *flatpack_id, char *filedata);

 
  /**
   * Get a flatpack using a domain name
   *
   *  @param flatpack_id - the id to search for
   *  @return - the data from the api
  */
  char * GETFlatpackInherit( char *flatpack_id);

 
  /**
   * returns the LESS theme from a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETFlatpackLess( char *flatpack_id);

 
  /**
   * Remove a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param gen_id - the id of the canned link to remove
   *  @return - the data from the api
  */
  char * DELETEFlatpackLink( char *flatpack_id, char *gen_id);

 
  /**
   * Add a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param keywords - the keywords to use in the canned search
   *  @param location - the location to use in the canned search
   *  @param linkText - the link text to be used to in the canned search link
   *  @return - the data from the api
  */
  char * POSTFlatpackLink( char *flatpack_id, char *keywords, char *location, char *linkText);

 
  /**
   * Remove all canned links from an existing flatpack.
   *
   *  @param flatpack_id - the id of the flatpack to remove links from
   *  @return - the data from the api
  */
  char * DELETEFlatpackLinkAll( char *flatpack_id);

 
  /**
   * Upload a logo to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackLogo( char *flatpack_id, char *filedata);

 
  /**
   * Add a hd logo to a flatpack domain
   *
   *  @param flatpack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackLogoHd( char *flatpack_id, char *filedata);

 
  /**
   * Delete a Redirect link from a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * DELETEFlatpackRedirect( char *flatpack_id);

 
  /**
   * Add a Redirect link to a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @param redirectTo
   *  @return - the data from the api
  */
  char * POSTFlatpackRedirect( char *flatpack_id, char *redirectTo);

 
  /**
   * Upload a TXT file to act as the sitemap for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTFlatpackSitemap( char *flatpack_id, char *filedata);

 
  /**
   * Delete a group with a specified group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * DELETEGroup( char *group_id);

 
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
  char * POSTGroup( char *group_id, char *name, char *description, char *url, char *stamp_user_id, char *stamp_sql);

 
  /**
   * Returns group that matches a given group id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * GETGroup( char *group_id);

 
  /**
   * Returns all groups
   *
   *  @return - the data from the api
  */
  char * GETGroupAll();

 
  /**
   * Bulk delete entities from a specified group
   *
   *  @param group_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTGroupBulk_delete( char *group_id, char *filedata);

 
  /**
   * Bulk ingest entities into a specified group
   *
   *  @param group_id
   *  @param filedata
   *  @param category_type
   *  @return - the data from the api
  */
  char * POSTGroupBulk_ingest( char *group_id, char *filedata, char *category_type);

 
  /**
   * Bulk update entities with a specified group
   *
   *  @param group_id
   *  @param data
   *  @return - the data from the api
  */
  char * POSTGroupBulk_update( char *group_id, char *data);

 
  /**
   * Get number of claims today
   *
   *  @param from_date
   *  @param to_date
   *  @param country_id
   *  @return - the data from the api
  */
  char * GETHeartbeatBy_date( char *from_date, char *to_date, char *country_id);

 
  /**
   * Get number of claims today
   *
   *  @param country
   *  @param claim_type
   *  @return - the data from the api
  */
  char * GETHeartbeatTodayClaims( char *country, char *claim_type);

 
  /**
   * Process a bulk file
   *
   *  @param job_id
   *  @param filedata - A tab separated file for ingest
   *  @return - the data from the api
  */
  char * POSTIngest_file( char *job_id, char *filedata);

 
  /**
   * Add a ingest job to the collection
   *
   *  @param description
   *  @param category_type
   *  @return - the data from the api
  */
  char * POSTIngest_job( char *description, char *category_type);

 
  /**
   * Get an ingest job from the collection
   *
   *  @param job_id
   *  @return - the data from the api
  */
  char * GETIngest_job( char *job_id);

 
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
  char * GETIngest_logBy_job_id( char *job_id, char *success, char *errors, char *limit, char *skip);

 
  /**
   * Check the status of the Ingest queue, and potentially flush it
   *
   *  @param flush
   *  @return - the data from the api
  */
  char * GETIngest_queue( char *flush);

 
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
  char * GETLeadsAdded( char *country_id, char *from_date, char *to_date, char *limit, char *offset, char *reduce);

 
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
  char * GETLeadsAdvertisers( char *country_id, char *from_date, char *to_date, char *limit, char *offset, char *reduce);

 
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
  char * GETLeadsClaimed( char *country_id, char *from_date, char *to_date, char *limit, char *offset, char *reduce);

 
  /**
   * Read a location with the supplied ID in the locations reference database.
   *
   *  @param location_id
   *  @return - the data from the api
  */
  char * GETLocation( char *location_id);

 
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
  char * POSTLocation( char *location_id, char *type, char *country, char *language, char *name, char *formal_name, char *resolution, char *population, char *description, char *timezone, char *latitude, char *longitude, char *parent_town, char *parent_county, char *parent_province, char *parent_region, char *parent_neighbourhood, char *parent_district, char *postalcode, char *searchable_id, char *searchable_ids);

 
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
  char * GETLocationContext( char *location_id, char *latitude, char *longitude, char *radius, char *resolution, char *country, char *num_results);

 
  /**
   * Read multiple locations with the supplied ID in the locations reference database.
   *
   *  @param location_ids
   *  @return - the data from the api
  */
  char * GETLocationMultiple( char *location_ids);

 
  /**
   * With a unique login_id a login can be retrieved
   *
   *  @param login_id
   *  @return - the data from the api
  */
  char * GETLogin( char *login_id);

 
  /**
   * Create/Update login details
   *
   *  @param login_id
   *  @param email
   *  @param password
   *  @return - the data from the api
  */
  char * POSTLogin( char *login_id, char *email, char *password);

 
  /**
   * With a unique login_id a login can be deleted
   *
   *  @param login_id
   *  @return - the data from the api
  */
  char * DELETELogin( char *login_id);

 
  /**
   * With a unique email address a login can be retrieved
   *
   *  @param email
   *  @return - the data from the api
  */
  char * GETLoginBy_email( char *email);

 
  /**
   * Verify that a supplied email and password match a users saved login details
   *
   *  @param email
   *  @param password
   *  @return - the data from the api
  */
  char * GETLoginVerify( char *email, char *password);

 
  /**
   * Fetch the project logo, the symbol of the Wolf
   *
   *  @param a
   *  @param b
   *  @param c
   *  @param d
   *  @return - the data from the api
  */
  char * GETLogo( char *a, char *b, char *c, char *d);

 
  /**
   * Fetch the project logo, the symbol of the Wolf
   *
   *  @param a
   *  @return - the data from the api
  */
  char * PUTLogo( char *a);

 
  /**
   * Find a category from cache or cloudant depending if it is in the cache
   *
   *  @param string - A string to search against, E.g. Plumbers
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * GETLookupCategory( char *string, char *language);

 
  /**
   * Find a category from a legacy ID or supplier (e.g. bill_moss)
   *
   *  @param id
   *  @param type
   *  @return - the data from the api
  */
  char * GETLookupLegacyCategory( char *id, char *type);

 
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
  char * GETLookupLocation( char *string, char *language, char *country, char *latitude, char *longitude);

 
  /**
   * Returns a list of mashery IDs domain names in which direct/inherited flatpack country match the specified one and status equals production.
   *
   *  @return - the data from the api
  */
  char * GETMasheryidAll();

 
  /**
   * Find all matches by phone number, returning up to 10 matches
   *
   *  @param phone
   *  @param country
   *  @param exclude - Entity ID to exclude from the results
   *  @return - the data from the api
  */
  char * GETMatchByphone( char *phone, char *country, char *exclude);

 
  /**
   * Perform a match on the two supplied entities, returning the outcome and showing our working
   *
   *  @param primary_entity_id
   *  @param secondary_entity_id
   *  @param return_entities - Should we return the entity documents
   *  @return - the data from the api
  */
  char * GETMatchOftheday( char *primary_entity_id, char *secondary_entity_id, char *return_entities);

 
  /**
   * Will create a new Matching Instruction or update an existing one
   *
   *  @param entity_id
   *  @param entity_name
   *  @return - the data from the api
  */
  char * POSTMatching_instruction( char *entity_id, char *entity_name);

 
  /**
   * Delete Matching instruction
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * DELETEMatching_instruction( char *entity_id);

 
  /**
   * Fetch all available Matching instructions
   *
   *  @param limit
   *  @return - the data from the api
  */
  char * GETMatching_instructionAll( char *limit);

 
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
  char * PUTMatching_log( char *primary_entity_id, char *secondary_entity_id, char *primary_name, char *secondary_name, char *address_score, char *address_match, char *name_score, char *name_match, char *distance, char *phone_match, char *category_match, char *email_match, char *website_match, char *match);

 
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
  char * POSTMaxclaimsActivate( char *user_id, char *contract_id, char *country, char *number, char *expiry_date);

 
  /**
   * Fetching a message
   *
   *  @param message_id - The message id to pull the message for
   *  @return - the data from the api
  */
  char * GETMessage( char *message_id);

 
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
  char * POSTMessage( char *message_id, char *ses_id, char *from_user_id, char *from_email, char *to_entity_id, char *to_email, char *subject, char *body, char *bounced);

 
  /**
   * Fetching messages by ses_id
   *
   *  @param ses_id - The amazon id to pull the message for
   *  @return - the data from the api
  */
  char * GETMessageBy_ses_id( char *ses_id);

 
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
  char * POSTMultipack( char *multipack_id, char *group_id, char *domainName, char *multipackName, char *less, char *country, char *menuTop, char *menuBottom, char *language, char *menuFooter, char *searchNumberResults, char *searchTitle, char *searchDescription, char *searchTitleNoWhere, char *searchDescriptionNoWhere, char *searchIntroHeader, char *searchIntroText, char *searchShowAll, char *searchUnitOfDistance, char *cookiePolicyShow, char *cookiePolicyUrl, char *twitterUrl, char *facebookUrl);

 
  /**
   * Get a multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETMultipack( char *multipack_id);

 
  /**
   * Add an admin theme to a multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTMultipackAdminCSS( char *multipack_id, char *filedata);

 
  /**
   * Add a Admin logo to a Multipack domain
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTMultipackAdminLogo( char *multipack_id, char *filedata);

 
  /**
   * Get a multipack using a domain name
   *
   *  @param domainName - the domain name to search for
   *  @return - the data from the api
  */
  char * GETMultipackBy_domain_name( char *domainName);

 
  /**
   * duplicates a given multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @param domainName - the domain name to serve this multipack site on (no leading http:// or anything please)
   *  @param group_id - the group to use for search
   *  @return - the data from the api
  */
  char * GETMultipackClone( char *multipack_id, char *domainName, char *group_id);

 
  /**
   * returns the LESS theme from a multipack
   *
   *  @param multipack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * GETMultipackLess( char *multipack_id);

 
  /**
   * Add a logo to a multipack domain
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTMultipackLogo( char *multipack_id, char *filedata);

 
  /**
   * Add a map pin to a multipack domain
   *
   *  @param multipack_id - the unique id to search for
   *  @param filedata
   *  @param mapPinOffsetX
   *  @param mapPinOffsetY
   *  @return - the data from the api
  */
  char * POSTMultipackMap_pin( char *multipack_id, char *filedata, char *mapPinOffsetX, char *mapPinOffsetY);

 
  /**
   * Fetch an ops_log
   *
   *  @param ops_log_id
   *  @return - the data from the api
  */
  char * GETOps_log( char *ops_log_id);

 
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
  char * POSTOps_log( char *success, char *type, char *action, char *data, char *slack_channel);

 
  /**
   * Run PTB for a given ingest job ID.
   *
   *  @param ingest_job_id - The ingest job ID
   *  @param email - When all entity IDs are pushed to the PTB queue, an email containing debug info will be sent.
   *  @return - the data from the api
  */
  char * POSTPaintBy_ingest_job_id( char *ingest_job_id, char *email);

 
  /**
   * With a known entity id syndication of data back to a partner is enabled
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateActivate( char *entity_id, char *publisher_id, char *expiry_date);

 
  /**
   * Call CK syndication instruction and call cancel endpoint for partner/supplier_id
   *
   *  @param supplierid
   *  @param vendor
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateCancel( char *supplierid, char *vendor);

 
  /**
   * This will call into CK in order to create the entity on the third party system.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateCreate( char *entity_id);

 
  /**
   * If this call fails CK is nudged for a human intervention for the future (so the call is NOT passive)
   *
   *  @param vendor_cat_id
   *  @param vendor_cat_string
   *  @param vendor
   *  @return - the data from the api
  */
  char * GETPartnersyndicateRequestcat( char *vendor_cat_id, char *vendor_cat_string, char *vendor);

 
  /**
   * This will do nothing if the entity does not have a current partnersyndicate block. Syndication is invoked automatically when entities are saved, so this endpoint is designed for checking the status of syndication.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTPartnersyndicateUpdateToCk( char *entity_id);

 
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
  char * POSTPlugin( char *id, char *slug, char *owner, char *scope, char *status, char *params);

 
  /**
   * Get plugin data
   *
   *  @param id
   *  @return - the data from the api
  */
  char * GETPlugin( char *id);

 
  /**
   * With a known entity id, a plugin is enabled
   *
   *  @param entity_id
   *  @param plugin
   *  @param inapp_name
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTPluginActivate( char *entity_id, char *plugin, char *inapp_name, char *expiry_date);

 
  /**
   * With a known entity id, a plugin is cancelled
   *
   *  @param entity_id
   *  @param plugin
   *  @param inapp_name
   *  @param expiry_date
   *  @return - the data from the api
  */
  char * POSTPluginCancel( char *entity_id, char *plugin, char *inapp_name, char *expiry_date);

 
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
  char * GETPluginDatarow( char *pluginid, char *name, char *filter1, char *filter2, char *order, char *fields);

 
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
  char * POSTPluginDatarow( char *rowdataid, char *pluginid, char *name, char *filter1, char *filter2, char *fields);

 
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
  char * POSTPluginVar( char *pluginid, char *userid, char *entity_id, char *storekey, char *storeval);

 
  /**
   * Get variables related to a particular entity
   *
   *  @param entityid
   *  @return - the data from the api
  */
  char * GETPluginVarsByEntityId( char *entityid);

 
  /**
   * Get info on all plugins
   *
   *  @return - the data from the api
  */
  char * GETPlugins();

 
  /**
   * Allows a private object to be removed
   *
   *  @param private_object_id - The id of the private object to remove
   *  @return - the data from the api
  */
  char * DELETEPrivate_object( char *private_object_id);

 
  /**
   * With a known entity id, a private object can be added.
   *
   *  @param entity_id - The entity to associate the private object with
   *  @param data - The data to store
   *  @return - the data from the api
  */
  char * PUTPrivate_object( char *entity_id, char *data);

 
  /**
   * Allows a private object to be returned based on the entity_id and masheryid
   *
   *  @param entity_id - The entity associated with the private object
   *  @return - the data from the api
  */
  char * GETPrivate_objectAll( char *entity_id);

 
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
  char * POSTProduct( char *product_id, char *shortname, char *name, char *strapline, char *alternate_title, char *fpzones, char *paygateid, char *price, char *tax_rate, char *currency, char *active, char *billing_period, char *title, char *intro_paragraph, char *bullets, char *outro_paragraph, char *product_description_html, char *thankyou_html, char *thanks_paragraph, char *video_url);

 
  /**
   * Returns the product information given a valid product_id
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * GETProduct( char *product_id);

 
  /**
   * Uploads logo image to product
   *
   *  @param product_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTProductImageLogo( char *product_id, char *filedata);

 
  /**
   * Delete the logo image within a specific product
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * DELETEProductImageLogo( char *product_id);

 
  /**
   * Delete the main image within a specific product
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * DELETEProductImageMain( char *product_id);

 
  /**
   * Adds partblahnersyndicate provisioning object to a product
   *
   *  @param product_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTProductImageMain( char *product_id, char *filedata);

 
  /**
   * Delete the small image within a specific product
   *
   *  @param product_id
   *  @return - the data from the api
  */
  char * DELETEProductImageSmall( char *product_id);

 
  /**
   * Uploads small image to product
   *
   *  @param product_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTProductImageSmall( char *product_id, char *filedata);

 
  /**
   * Removes a provisioning object from product
   *
   *  @param product_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * DELETEProductProvisioning( char *product_id, char *gen_id);

 
  /**
   * Adds advertising provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @param max_tags
   *  @param max_locations
   *  @return - the data from the api
  */
  char * POSTProductProvisioningAdvert( char *product_id, char *publisher_id, char *max_tags, char *max_locations);

 
  /**
   * Adds claim provisioning object to a product
   *
   *  @param product_id
   *  @param package
   *  @return - the data from the api
  */
  char * POSTProductProvisioningClaim( char *product_id, char *package);

 
  /**
   * Adds maxclaims provisioning object to a product
   *
   *  @param product_id
   *  @param country
   *  @param number
   *  @return - the data from the api
  */
  char * POSTProductProvisioningMaxclaims( char *product_id, char *country, char *number);

 
  /**
   * Adds partnersyndicate provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTProductProvisioningPartnersyndicate( char *product_id, char *publisher_id);

 
  /**
   * Adds plugin provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTProductProvisioningPlugin( char *product_id, char *publisher_id);

 
  /**
   * Adds SCheduleSMS provisioning object to a product
   *
   *  @param product_id
   *  @param package
   *  @return - the data from the api
  */
  char * POSTProductProvisioningSchedulesms( char *product_id, char *package);

 
  /**
   * Adds syndication provisioning object to a product
   *
   *  @param product_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * POSTProductProvisioningSyndication( char *product_id, char *publisher_id);

 
  /**
   * Perform the whole PTB process on the supplied entity
   *
   *  @param entity_id
   *  @param destructive
   *  @return - the data from the api
  */
  char * GETPtbAll( char *entity_id, char *destructive);

 
  /**
   * Report on what happened to specific entity_id
   *
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param entity_id - the entity to research
   *  @return - the data from the api
  */
  char * GETPtbLog( char *year, char *month, char *entity_id);

 
  /**
   * Process an entity with a specific PTB module
   *
   *  @param entity_id
   *  @param module
   *  @param destructive
   *  @return - the data from the api
  */
  char * GETPtbModule( char *entity_id, char *module, char *destructive);

 
  /**
   * Report on the run-rate of the Paint the Bridge System
   *
   *  @param country - the country to get the runrate for
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param day - the day to examine
   *  @return - the data from the api
  */
  char * GETPtbRunrate( char *country, char *year, char *month, char *day);

 
  /**
   * Report on the value being added by Paint The Bridge
   *
   *  @param country - the country to get the runrate for
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param day - the day to examine
   *  @return - the data from the api
  */
  char * GETPtbValueadded( char *country, char *year, char *month, char *day);

 
  /**
   * Returns publisher that matches a given publisher id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * GETPublisher( char *publisher_id);

 
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
  char * POSTPublisher( char *publisher_id, char *country, char *name, char *description, char *active, char *url_patterns, char *premium_adverts_platinum, char *premium_adverts_gold);

 
  /**
   * Delete a publisher with a specified publisher_id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * DELETEPublisher( char *publisher_id);

 
  /**
   * Returns publisher that matches a given publisher id
   *
   *  @param country
   *  @return - the data from the api
  */
  char * GETPublisherByCountry( char *country);

 
  /**
   * Returns publishers that are available for a given entity_id.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETPublisherByEntityId( char *entity_id);

 
  /**
   * Returns a publisher that has the specified masheryid
   *
   *  @param publisher_masheryid
   *  @return - the data from the api
  */
  char * GETPublisherBy_masheryid( char *publisher_masheryid);

 
  /**
   * Retrieve queue items.
   *
   *  @param limit
   *  @param queue_name
   *  @return - the data from the api
  */
  char * GETQueue( char *limit, char *queue_name);

 
  /**
   * Create a queue item
   *
   *  @param queue_name
   *  @param data
   *  @return - the data from the api
  */
  char * PUTQueue( char *queue_name, char *data);

 
  /**
   * With a known queue id, a queue item can be removed.
   *
   *  @param queue_id
   *  @return - the data from the api
  */
  char * DELETEQueue( char *queue_id);

 
  /**
   * Find a queue item by its cloudant id
   *
   *  @param queue_id
   *  @return - the data from the api
  */
  char * GETQueueBy_id( char *queue_id);

 
  /**
   * Add an error to a queue item
   *
   *  @param queue_id
   *  @param error
   *  @return - the data from the api
  */
  char * POSTQueueError( char *queue_id, char *error);

 
  /**
   * Find a queue item by its type and id
   *
   *  @param type
   *  @param id
   *  @return - the data from the api
  */
  char * GETQueueSearch( char *type, char *id);

 
  /**
   * Unlock queue items.
   *
   *  @param queue_name
   *  @param seconds
   *  @return - the data from the api
  */
  char * POSTQueueUnlock( char *queue_name, char *seconds);

 
  /**
   * Create an SQS queue item
   *
   *  @param queue_name
   *  @param data
   *  @return - the data from the api
  */
  char * PUTQueue_sqs( char *queue_name, char *data);

 
  /**
   * Get the attributes of an SQS queue
   *
   *  @param queue_name
   *  @return - the data from the api
  */
  char * GETQueue_sqsAttributes( char *queue_name);

 
  /**
   * Returns reseller that matches a given reseller id
   *
   *  @param reseller_id
   *  @return - the data from the api
  */
  char * GETReseller( char *reseller_id);

 
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
  char * POSTReseller( char *reseller_id, char *country, char *name, char *description, char *active, char *products, char *master_user_id, char *canViewEmployee);

 
  /**
   * Return a sales log by id
   *
   *  @param sales_log_id - The sales log id to pull
   *  @return - the data from the api
  */
  char * GETSales_log( char *sales_log_id);

 
  /**
   * Return a sales log by id
   *
   *  @param from_date
   *  @param country
   *  @param action_type
   *  @return - the data from the api
  */
  char * GETSales_logBy_countryBy_date( char *from_date, char *country, char *action_type);

 
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
  char * GETSales_logBy_date( char *from_date, char *to_date, char *group, char *limit, char *skip);

 
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
  char * POSTSales_logEntity( char *entity_id, char *country, char *action_type, char *ad_type, char *publisher_id, char *mashery_id, char *reseller_ref, char *reseller_agent_id, char *max_tags, char *max_locations, char *extra_tags, char *extra_locations, char *expiry_date);

 
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
  char * POSTSales_logSyndication( char *action_type, char *syndication_type, char *publisher_id, char *expiry_date, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *reseller_masheryid);

 
  /**
   * Converts an Entity into a submission at the Scoot Partner API
   *
   *  @param entity_id - The entity to parse
   *  @param reseller - The reseller Mashery ID, it also determines which Scoot API key to use
   *  @param scoot_id - If specified, the related Scoot listing will be updated.
   *  @param autofill_scoot_id - If scoot_id is not given, look for past successful syndication logs to fill in the Scoot ID
   *  @return - the data from the api
  */
  char * POSTScoot_priority( char *entity_id, char *reseller, char *scoot_id, char *autofill_scoot_id);

 
  /**
   * Make a url shorter
   *
   *  @param url - the url to shorten
   *  @param idOnly - Return just the Shortened URL ID
   *  @return - the data from the api
  */
  char * PUTShortenurl( char *url, char *idOnly);

 
  /**
   * get the long url from the db
   *
   *  @param id - the id to fetch from the db
   *  @return - the data from the api
  */
  char * GETShortenurl( char *id);

 
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
  char * POSTSignal( char *entity_id, char *country, char *gen_id, char *signal_type, char *data_type, char *inactive_reason, char *inactive_description, char *feedback);

 
  /**
   * With a given country and entity id suffix, this endpoint will return a list of entity IDs and their last modified dates for sitemap generation.
   *
   *  @param country - Target country code.
   *  @param id_suffix - Target entity Id suffix (4 digits).
   *  @param skip
   *  @param limit
   *  @return - the data from the api
  */
  char * GETSitemapEntity( char *country, char *id_suffix, char *skip, char *limit);

 
  /**
   * With a given country, this endpoint will return a list of entity ID suffixes which have records.
   *
   *  @param country - Target country code.
   *  @return - the data from the api
  */
  char * GETSitemapEntitySummary( char *country);

 
  /**
   * Get a spider document
   *
   *  @param spider_id
   *  @return - the data from the api
  */
  char * GETSpider( char *spider_id);

 
  /**
   * Get the number of times an entity has been served out as an advert or on serps/bdp pages
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @param month - The month to report on
   *  @return - the data from the api
  */
  char * GETStatsEntityBy_date( char *entity_id, char *year, char *month);

 
  /**
   * Get the stats on an entity in a given year
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @return - the data from the api
  */
  char * GETStatsEntityBy_year( char *entity_id, char *year);

 
  /**
   * Confirms that the API is active, and returns the current version number
   *
   *  @return - the data from the api
  */
  char * GETStatus();

 
  /**
   * get a Syndication
   *
   *  @param syndication_id
   *  @return - the data from the api
  */
  char * GETSyndication( char *syndication_id);

 
  /**
   * get a Syndication by entity_id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETSyndicationBy_entity_id( char *entity_id);

 
  /**
   * Get a Syndication by Reseller (Mashery ID) and optional entity ID
   *
   *  @param reseller_masheryid
   *  @param entity_id
   *  @return - the data from the api
  */
  char * GETSyndicationBy_reseller( char *reseller_masheryid, char *entity_id);

 
  /**
   * Cancel a syndication
   *
   *  @param syndication_id
   *  @return - the data from the api
  */
  char * POSTSyndicationCancel( char *syndication_id);

 
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
  char * POSTSyndicationCreate( char *syndication_type, char *publisher_id, char *expiry_date, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *data_filter);

 
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
  char * POSTSyndicationRenew( char *syndication_type, char *publisher_id, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *expiry_date);

 
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
  char * POSTSyndication_log( char *entity_id, char *publisher_id, char *action, char *success, char *message, char *syndicated_id, char *reseller_id);

 
  /**
   * Get all syndication log entries for a given entity id
   *
   *  @param entity_id
   *  @param page
   *  @param per_page
   *  @return - the data from the api
  */
  char * GETSyndication_logBy_entity_id( char *entity_id, char *page, char *per_page);

 
  /**
   * Get the latest syndication log feedback entry for a given entity id and publisher
   *
   *  @param entity_id
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * GETSyndication_logLast_syndicated_id( char *entity_id, char *publisher_id);

 
  /**
   * Creates a new Syndication Submission
   *
   *  @param syndication_type
   *  @param entity_id
   *  @param publisher_id
   *  @param submission_id
   *  @return - the data from the api
  */
  char * PUTSyndication_submission( char *syndication_type, char *entity_id, char *publisher_id, char *submission_id);

 
  /**
   * Returns a Syndication Submission
   *
   *  @param syndication_submission_id
   *  @return - the data from the api
  */
  char * GETSyndication_submission( char *syndication_submission_id);

 
  /**
   * Set active to false for a Syndication Submission
   *
   *  @param syndication_submission_id
   *  @return - the data from the api
  */
  char * POSTSyndication_submissionDeactivate( char *syndication_submission_id);

 
  /**
   * Set the processed date to now for a Syndication Submission
   *
   *  @param syndication_submission_id
   *  @return - the data from the api
  */
  char * POSTSyndication_submissionProcessed( char *syndication_submission_id);

 
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
  char * GETTokenAdd( char *language, char *business_name, char *business_phone, char *business_postcode, char *portal_name, char *supplier_id, char *partner, char *country, char *flatpack_id);

 
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
  char * GETTokenClaim( char *entity_id, char *supplier_id, char *language, char *portal_name, char *flatpack_id, char *admin_host);

 
  /**
   * Fetch token for the contact us method
   *
   *  @param portal_name - The portal name
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param language - en, es, etc...
   *  @param referring_url - the url where the request came from
   *  @return - the data from the api
  */
  char * GETTokenContact_us( char *portal_name, char *flatpack_id, char *language, char *referring_url);

 
  /**
   * Allows us to identify the user, entity and element from an encoded endpoint URL's token
   *
   *  @param token
   *  @return - the data from the api
  */
  char * GETTokenDecode( char *token);

 
  /**
   * Fetch token for edit path
   *
   *  @param entity_id - The id of the entity being upgraded
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param edit_page - the page in the edit path that the user should land on
   *  @return - the data from the api
  */
  char * GETTokenEdit( char *entity_id, char *language, char *flatpack_id, char *edit_page);

 
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
  char * GETTokenEncode( char *portal_name, char *code, char *expireAt, char *language, char *flatpack_id, char *multipack_id, char *data);

 
  /**
   * Fetch token for login path
   *
   *  @param portal_name - The name of the application that has initiated the login process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param multipack_id - The id of the multipack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenLogin( char *portal_name, char *language, char *flatpack_id, char *multipack_id);

 
  /**
   * Get a tokenised url for an password reset
   *
   *  @param login_id - Login id
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param entity_id
   *  @param action
   *  @return - the data from the api
  */
  char * GETTokenLoginReset_password( char *login_id, char *flatpack_id, char *entity_id, char *action);

 
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
  char * GETTokenLoginSet_password( char *email, char *first_name, char *last_name, char *flatpack_id, char *entity_id, char *action);

 
  /**
   * Fetch token for messaging path
   *
   *  @param entity_id - The id of the entity being messaged
   *  @param portal_name - The name of the application that has initiated the email process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenMessage( char *entity_id, char *portal_name, char *language, char *flatpack_id);

 
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
  char * GETTokenPartnerclaim( char *language, char *flatpack_id, char *partner, char *partnerid, char *preclaimed);

 
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
  char * GETTokenPartnerclaimInternal( char *language, char *flatpack_id, char *partner, char *entityid, char *preclaimed);

 
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
  char * GETTokenProduct( char *entity_id, char *product_id, char *language, char *portal_name, char *flatpack_id, char *source, char *channel, char *campaign);

 
  /**
   * Fetch token for product path
   *
   *  @param entity_id - The id of the entity to add a product to
   *  @param portal_name - The portal name
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param language - en, es, etc...
   *  @return - the data from the api
  */
  char * GETTokenProduct_selector( char *entity_id, char *portal_name, char *flatpack_id, char *language);

 
  /**
   * Provides a tokenised URL that allows a user to report incorrect entity information
   *
   *  @param entity_id - The unique Entity ID e.g. 379236608286720
   *  @param portal_name - The name of the portal that the user is coming from e.g. YourLocal
   *  @param language - The language to use to render the report path
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenReport( char *entity_id, char *portal_name, char *language, char *flatpack_id);

 
  /**
   * Get a tokenised url for the review path
   *
   *  @param portal_name - The portal name
   *  @param entity_id
   *  @param language - en, es, etc...
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * GETTokenReview( char *portal_name, char *entity_id, char *language, char *flatpack_id);

 
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
  char * GETTokenTestimonial( char *portal_name, char *flatpack_id, char *language, char *entity_id, char *shorten_url);

 
  /**
   * The JaroWinklerDistance between two entities postal address objects
   *
   *  @param first_entity_id - The entity id of the first entity to be used in the postal address difference
   *  @param second_entity_id - The entity id of the second entity to be used in the postal address difference
   *  @return - the data from the api
  */
  char * GETToolsAddressdiff( char *first_entity_id, char *second_entity_id);

 
  /**
   * An API call to test crashing the server
   *
   *  @return - the data from the api
  */
  char * GETToolsCrash();

 
  /**
   * Provide a method, a path and some data to run a load of curl commands and get emailed when complete
   *
   *  @param method - The method e.g. POST
   *  @param path - The relative api call e.g. /entity/phone
   *  @param filedata - A tab separated file for ingest
   *  @param email - Response email address e.g. dave@fender.com
   *  @return - the data from the api
  */
  char * POSTToolsCurl( char *method, char *path, char *filedata, char *email);

 
  /**
   * Use this call to get information (in HTML or JSON) about the data structure of given entity object (e.g. a phone number or an address)
   *
   *  @param object - The API call documentation is required for
   *  @param format - The format of the returned data eg. JSON or HTML
   *  @return - the data from the api
  */
  char * GETToolsDocs( char *object, char *format);

 
  /**
   * Format an address according to the rules of the country supplied
   *
   *  @param address - The address to format
   *  @param country - The country where the address is based
   *  @return - the data from the api
  */
  char * GETToolsFormatAddress( char *address, char *country);

 
  /**
   * Format a phone number according to the rules of the country supplied
   *
   *  @param number - The telephone number to format
   *  @param country - The country where the telephone number is based
   *  @param ignoreRegionCheck - If ture, we only check if the phone number is valid, ignoring country context
   *  @return - the data from the api
  */
  char * GETToolsFormatPhone( char *number, char *country, char *ignoreRegionCheck);

 
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
  char * GETToolsGeocode( char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country);

 
  /**
   * Given a spreadsheet ID, and a worksheet ID, add a row
   *
   *  @param spreadsheet_key - The key of the spreadsheet to edit
   *  @param worksheet_key - The key of the worksheet to edit - failure to provide this will assume worksheet with the label 'Sheet1'
   *  @param data - A comma separated list to add as cells
   *  @return - the data from the api
  */
  char * POSTToolsGooglesheetAdd_row( char *spreadsheet_key, char *worksheet_key, char *data);

 
  /**
   * Given a spreadsheet ID and the name of a worksheet identify the Google ID for the worksheet
   *
   *  @param spreadsheet_key - The key of the spreadsheet
   *  @param worksheet_name - The name/label of the worksheet to identify
   *  @return - the data from the api
  */
  char * POSTToolsGooglesheetWorksheet_id( char *spreadsheet_key, char *worksheet_name);

 
  /**
   * Given some image data we can resize and upload the images
   *
   *  @param filedata - The image data to upload and resize
   *  @param type - The type of image to upload and resize
   *  @param image_dir - Set the destination directory of the generated images on S3. Only available when Image API is enabled.
   *  @return - the data from the api
  */
  char * POSTToolsImage( char *filedata, char *type, char *image_dir);

 
  /**
   * Generate JSON in the format to generate Mashery's IODocs
   *
   *  @param mode - The HTTP method of the API call to document. e.g. GET
   *  @param path - The path of the API call to document e.g, /entity
   *  @param endpoint - The Mashery 'endpoint' to prefix to our API paths e.g. v1
   *  @param doctype - Mashery has two forms of JSON to describe API methods; one on github, the other on its customer dashboard
   *  @return - the data from the api
  */
  char * GETToolsIodocs( char *mode, char *path, char *endpoint, char *doctype);

 
  /**
   * compile the supplied less with the standard Bootstrap less into a CSS file
   *
   *  @param less - The LESS code to compile
   *  @return - the data from the api
  */
  char * GETToolsLess( char *less);

 
  /**
   * Parse unstructured address data to fit our structured address objects
   *
   *  @param address
   *  @param postcode
   *  @param country
   *  @param normalise
   *  @return - the data from the api
  */
  char * GETToolsParseAddress( char *address, char *postcode, char *country, char *normalise);

 
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
  char * GETToolsPhonecallVerify( char *to, char *from, char *pin, char *twilio_voice, char *extension);

 
  /**
   * Return the phonetic representation of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsPhonetic( char *text);

 
  /**
   * Attempt to process a phone number, removing anything which is not a digit
   *
   *  @param number
   *  @return - the data from the api
  */
  char * GETToolsProcess_phone( char *number);

 
  /**
   * Fully process a string. This includes removing punctuation, stops words and stemming a string. Also returns the phonetic representation of this string.
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsProcess_string( char *text);

 
  /**
   * Force refresh of search indexes
   *
   *  @return - the data from the api
  */
  char * GETToolsReindex();

 
  /**
   * Check to see if a supplied email address is valid
   *
   *  @param from - The phone number from which the SMS orginates
   *  @param to - The phone number to which the SMS is to be sent
   *  @param message - The message to be sent in the SMS
   *  @return - the data from the api
  */
  char * GETToolsSendsms( char *from, char *to, char *message);

 
  /**
   * Spider a single url looking for key facts
   *
   *  @param url
   *  @param pages
   *  @param country
   *  @param save
   *  @return - the data from the api
  */
  char * GETToolsSpider( char *url, char *pages, char *country, char *save);

 
  /**
   * Returns a stemmed version of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsStem( char *text);

 
  /**
   * Removes stopwords from a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * GETToolsStopwords( char *text);

 
  /**
   * Fetch the result of submitted data we sent to InfoGroup
   *
   *  @param syndication_submission_id - The syndication_submission_id to fetch info for
   *  @return - the data from the api
  */
  char * GETToolsSubmissionInfogroup( char *syndication_submission_id);

 
  /**
   * Fetch the entity and convert it to 118 Places CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicate118( char *entity_id);

 
  /**
   * Fetch the entity and convert it to Bing Ads CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateBingads( char *entity_id);

 
  /**
   * Fetch the entity and convert it to Bing Places CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateBingplaces( char *entity_id);

 
  /**
   * Fetch the entity and convert it to DnB TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateDnb( char *entity_id);

 
  /**
   * Fetch the entity and convert add it to arlington
   *
   *  @param entity_id - The entity_id to fetch
   *  @param reseller_masheryid - The reseller_masheryid
   *  @param destructive - Add the entity or simulate adding the entity
   *  @param data_filter - The level of filtering to apply to the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateEnablemedia( char *entity_id, char *reseller_masheryid, char *destructive, char *data_filter);

 
  /**
   * Fetch the entity and convert add it to Factual
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateFactual( char *entity_id, char *destructive);

 
  /**
   * Fetch the entity and convert it to Factual CSV / TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateFactualcsv( char *entity_id);

 
  /**
   * Syndicate an entity to Foursquare
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateFoursquare( char *entity_id, char *destructive);

 
  /**
   * Fetch the entity and convert it to TomTom XML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateGoogle( char *entity_id);

 
  /**
   * Fetch the entity and convert it to Infobel CSV / TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateInfobelcsv( char *entity_id);

 
  /**
   * Fetch the entity and convert add it to InfoGroup
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateInfogroup( char *entity_id, char *destructive);

 
  /**
   * Fetch the entity and convert add it to Judy's Book
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateJudysbook( char *entity_id, char *destructive);

 
  /**
   * Fetch the entity and convert it to Google KML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateKml( char *entity_id);

 
  /**
   * Syndicate database to localdatabase.com
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateLocaldatabase( char *entity_id, char *destructive);

 
  /**
   * Fetch the entity and convert it to Nokia NBS CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateNokia( char *entity_id);

 
  /**
   * Post an entity to OpenStreetMap
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateOsm( char *entity_id, char *destructive);

 
  /**
   * Syndicate an entity to ThomsonLocal
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateThomsonlocal( char *entity_id, char *destructive);

 
  /**
   * Fetch the entity and convert it to TomTom XML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateTomtom( char *entity_id);

 
  /**
   * Fetch the entity and convert it to YALWA csv
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * GETToolsSyndicateYalwa( char *entity_id);

 
  /**
   * Fetch the entity and convert add it to Yassaaaabeeee!!
   *
   *  @param entity_id - The entity_id to fetch
   *  @param destructive - Add the entity or simulate adding the entity
   *  @return - the data from the api
  */
  char * GETToolsSyndicateYasabe( char *entity_id, char *destructive);

 
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
  char * GETToolsTestmatch( char *name, char *building_number, char *branch_name, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *additional_telephone_number, char *email, char *website, char *category_id, char *category_type, char *do_not_display, char *referrer_url, char *referrer_name);

 
  /**
   * Send a transactional email via Adestra or other email provider
   *
   *  @param email_id - The ID of the email to send
   *  @param destination_email - The email address to send to
   *  @param email_supplier - The email supplier
   *  @return - the data from the api
  */
  char * GETToolsTransactional_email( char *email_id, char *destination_email, char *email_supplier);

 
  /**
   * Upload a file to our asset server and return the url
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * POSTToolsUpload( char *filedata);

 
  /**
   * Find a canonical URL from a supplied URL
   *
   *  @param url - The url to process
   *  @param max_redirects - The maximum number of reirects
   *  @return - the data from the api
  */
  char * GETToolsUrl_details( char *url, char *max_redirects);

 
  /**
   * Check to see if a supplied email address is valid
   *
   *  @param email_address - The email address to validate
   *  @return - the data from the api
  */
  char * GETToolsValidate_email( char *email_address);

 
  /**
   * Calls a number to make sure it is active
   *
   *  @param phone_number - The phone number to validate
   *  @param country - The country code of the phone number to be validated
   *  @return - the data from the api
  */
  char * GETToolsValidate_phone( char *phone_number, char *country);

 
  /**
   * Deleting a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * DELETETraction( char *traction_id);

 
  /**
   * Fetching a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * GETTraction( char *traction_id);

 
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
  char * POSTTraction( char *traction_id, char *trigger_type, char *action_type, char *country, char *email_addresses, char *title, char *body, char *api_method, char *api_url, char *api_params, char *active, char *reseller_masheryid, char *publisher_masheryid, char *description);

 
  /**
   * Fetching active tractions
   *
   *  @return - the data from the api
  */
  char * GETTractionActive();

 
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
  char * PUTTransaction( char *entity_id, char *user_id, char *basket_total, char *basket, char *currency, char *notes);

 
  /**
   * Given a transaction_id retrieve information on it
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * GETTransaction( char *transaction_id);

 
  /**
   * Set a transactions status to authorised
   *
   *  @param transaction_id
   *  @param paypal_getexpresscheckoutdetails
   *  @return - the data from the api
  */
  char * POSTTransactionAuthorised( char *transaction_id, char *paypal_getexpresscheckoutdetails);

 
  /**
   * Given a transaction_id retrieve information on it
   *
   *  @param paypal_transaction_id
   *  @return - the data from the api
  */
  char * GETTransactionBy_paypal_transaction_id( char *paypal_transaction_id);

 
  /**
   * Set a transactions status to cancelled
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * POSTTransactionCancelled( char *transaction_id);

 
  /**
   * Set a transactions status to complete
   *
   *  @param transaction_id
   *  @param paypal_doexpresscheckoutpayment
   *  @param user_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * POSTTransactionComplete( char *transaction_id, char *paypal_doexpresscheckoutpayment, char *user_id, char *entity_id);

 
  /**
   * Set a transactions status to inprogess
   *
   *  @param transaction_id
   *  @param paypal_setexpresscheckout
   *  @return - the data from the api
  */
  char * POSTTransactionInprogress( char *transaction_id, char *paypal_setexpresscheckout);

 
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
  char * POSTUser( char *email, char *user_id, char *first_name, char *last_name, char *active, char *last_flatpack, char *trust, char *creation_date, char *user_type, char *social_network, char *social_network_id, char *reseller_admin_masheryid, char *group_id, char *admin_upgrader, char *opt_in_marketing);

 
  /**
   * With a unique ID address an user can be retrieved
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETUser( char *user_id);

 
  /**
   * Is this user allowed to edit this entity
   *
   *  @param entity_id
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETUserAllowed_to_edit( char *entity_id, char *user_id);

 
  /**
   * With a unique email address an user can be retrieved
   *
   *  @param email
   *  @return - the data from the api
  */
  char * GETUserBy_email( char *email);

 
  /**
   * Returns all the users that match the supplied group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * GETUserBy_groupid( char *group_id);

 
  /**
   * Returns all the users that match the supplied reseller_admin_masheryid
   *
   *  @param reseller_admin_masheryid
   *  @return - the data from the api
  */
  char * GETUserBy_reseller_admin_masheryid( char *reseller_admin_masheryid);

 
  /**
   * With a unique ID address an user can be retrieved
   *
   *  @param name
   *  @param id
   *  @return - the data from the api
  */
  char * GETUserBy_social_media( char *name, char *id);

 
  /**
   * Downgrade an existing user
   *
   *  @param user_id
   *  @param user_type
   *  @return - the data from the api
  */
  char * POSTUserDowngrade( char *user_id, char *user_type);

 
  /**
   * Removes group_admin privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * POSTUserGroup_admin_remove( char *user_id);

 
  /**
   * Log user activities into MariaDB
   *
   *  @param user_id
   *  @param action_type
   *  @param domain
   *  @param time
   *  @return - the data from the api
  */
  char * POSTUserLog_activity( char *user_id, char *action_type, char *domain, char *time);

 
  /**
   * List user activity times within given date range (between inclusive from and exclusive to)
   *
   *  @param user_id
   *  @param action_type
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * GETUserLog_activityList_time( char *user_id, char *action_type, char *from, char *to);

 
  /**
   * Retrieve list of entities that the user manages
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * GETUserManaged_entities( char *user_id);

 
  /**
   * Removes reseller privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * POSTUserReseller_remove( char *user_id);

 
  /**
   * Deletes a specific social network from a user
   *
   *  @param user_id
   *  @param social_network
   *  @return - the data from the api
  */
  char * DELETEUserSocial_network( char *user_id, char *social_network);

 
  /**
   * Shows what would be emitted by a view, given a document
   *
   *  @param database - the database being worked on e.g. entities
   *  @param designdoc - the design document containing the view e.g. _design/report
   *  @param view - the name of the view to be tested e.g. bydate
   *  @param doc - the JSON document to be analysed e.g. {}
   *  @return - the data from the api
  */
  char * GETViewhelper( char *database, char *designdoc, char *view, char *doc);

 
  /**
   * Converts an Entity into webcard JSON and then doing a PUT /webcard/json
   *
   *  @param entity_id - The entity to create on webcard
   *  @return - the data from the api
  */
  char * POSTWebcard( char *entity_id);

 



