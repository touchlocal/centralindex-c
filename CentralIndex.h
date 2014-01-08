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
  char * getActivity_stream( char *type, char *country, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *number_results, char *unique_action);

 
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
  char * postActivity_stream( char *entity_id, char *entity_name, char *type, char *country, char *longitude, char *latitude);

 
  /**
   * Get all advertisers that have been updated from a give date for a given reseller
   *
   *  @param from_date
   *  @param country
   *  @return - the data from the api
  */
  char * getAdvertiserUpdated( char *from_date, char *country);

 
  /**
   * Get all advertisers that have been updated from a give date for a given publisher
   *
   *  @param publisher_id
   *  @param from_date
   *  @param country
   *  @return - the data from the api
  */
  char * getAdvertiserUpdatedBy_publisher( char *publisher_id, char *from_date, char *country);

 
  /**
   * The search matches a category name on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * getAutocompleteCategory( char *str, char *language);

 
  /**
   * The search matches a category name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Plumbers e.g. but
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * getAutocompleteKeyword( char *str, char *language);

 
  /**
   * The search matches a location name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Dub e.g. dub
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en e.g. en
   *  @return - the data from the api
  */
  char * getAutocompleteLocation( char *str, char *country, char *language);

 
  /**
   * The search matches a location name or synonym on a given string and language.
   *
   *  @param str - A string to search against, E.g. Middle e.g. dub
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param resolution
   *  @return - the data from the api
  */
  char * getAutocompleteLocationBy_resolution( char *str, char *country, char *resolution);

 
  /**
   * Create a new business entity with all it's objects
   *
   *  @param name
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
  char * putBusiness( char *name, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country, char *latitude, char *longitude, char *timezone, char *telephone_number, char *additional_telephone_number, char *email, char *website, char *category_id, char *category_type, char *do_not_display, char *referrer_url, char *referrer_name);

 
  /**
   * Returns business tool that matches a given tool id
   *
   *  @param tool_id
   *  @return - the data from the api
  */
  char * getBusiness_tool( char *tool_id);

 
  /**
   * Delete a business tool with a specified tool_id
   *
   *  @param tool_id
   *  @return - the data from the api
  */
  char * deleteBusiness_tool( char *tool_id);

 
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
  char * postBusiness_tool( char *tool_id, char *country, char *headline, char *description, char *link_url, char *active);

 
  /**
   * Returns active business tools for a specific masheryid in a given country
   *
   *  @param country
   *  @return - the data from the api
  */
  char * getBusiness_toolBy_masheryid( char *country);

 
  /**
   * Assigns a Business Tool image
   *
   *  @param tool_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * postBusiness_toolImage( char *tool_id, char *filedata);

 
  /**
   * With a known category id, an category object can be added.
   *
   *  @param category_id
   *  @param language
   *  @param name
   *  @return - the data from the api
  */
  char * putCategory( char *category_id, char *language, char *name);

 
  /**
   * Returns the supplied wolf category object by fetching the supplied category_id from our categories object.
   *
   *  @param category_id
   *  @return - the data from the api
  */
  char * getCategory( char *category_id);

 
  /**
   * Returns all Central Index categories and associated data
   *
   *  @return - the data from the api
  */
  char * getCategoryAll();

 
  /**
   * With a known category id, a mapping object can be added.
   *
   *  @param category_id
   *  @param type
   *  @param id
   *  @param name
   *  @return - the data from the api
  */
  char * postCategoryMappings( char *category_id, char *type, char *id, char *name);

 
  /**
   * With a known category id, a mapping object can be deleted.
   *
   *  @param category_id
   *  @param category_type
   *  @param mapped_id
   *  @return - the data from the api
  */
  char * deleteCategoryMappings( char *category_id, char *category_type, char *mapped_id);

 
  /**
   * Allows a category object to merged with another
   *
   *  @param from
   *  @param to
   *  @return - the data from the api
  */
  char * postCategoryMerge( char *from, char *to);

 
  /**
   * With a known category id, an synonym object can be added.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * postCategorySynonym( char *category_id, char *synonym, char *language);

 
  /**
   * With a known category id, a synonyms object can be removed.
   *
   *  @param category_id
   *  @param synonym
   *  @param language
   *  @return - the data from the api
  */
  char * deleteCategorySynonym( char *category_id, char *synonym, char *language);

 
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
  char * postCountry( char *country_id, char *name, char *synonyms, char *continentName, char *continent, char *geonameId, char *dbpediaURL, char *freebaseURL, char *population, char *currencyCode, char *languages, char *areaInSqKm, char *capital, char *east, char *west, char *north, char *south, char *claimPrice, char *claimMethods, char *twilio_sms, char *twilio_phone, char *twilio_voice, char *currency_symbol, char *currency_symbol_html, char *postcodeLookupActive, char *addressFields, char *addressMatching, char *dateFormat, char *iso_3166_alpha_3, char *iso_3166_numeric);

 
  /**
   * Fetching a country
   *
   *  @param country_id
   *  @return - the data from the api
  */
  char * getCountry( char *country_id);

 
  /**
   * For a given country add/update a background image to show in the add/edit path
   *
   *  @param country_id
   *  @param filedata
   *  @param backgroundImageAttr
   *  @return - the data from the api
  */
  char * postCountryBackgroundImage( char *country_id, char *filedata, char *backgroundImageAttr);

 
  /**
   * For a given country add/update a social login background image to show in the add/edit path
   *
   *  @param country_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * postCountrySocialLoginImage( char *country_id, char *filedata);

 
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
  char * postEmail( char *to_email_address, char *reply_email_address, char *source_account, char *subject, char *body, char *html_body);

 
  /**
   * Allows a whole entity to be pulled from the datastore by its unique id
   *
   *  @param entity_id - The unique entity ID e.g. 379236608286720
   *  @return - the data from the api
  */
  char * getEntity( char *entity_id);

 
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
  char * putEntity( char *type, char *scope, char *country, char *trust, char *our_data);

 
  /**
   * With a known entity id, an add can be updated.
   *
   *  @param entity_id
   *  @param add_referrer_url
   *  @param add_referrer_name
   *  @return - the data from the api
  */
  char * postEntityAdd( char *entity_id, char *add_referrer_url, char *add_referrer_name);

 
  /**
   * Allows an advertiser object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityAdvertiser( char *entity_id, char *gen_id);

 
  /**
   * Expires an advertiser from and entity
   *
   *  @param entity_id
   *  @param publisher_id
   *  @param reseller_ref
   *  @param reseller_agent_id
   *  @return - the data from the api
  */
  char * postEntityAdvertiserCancel( char *entity_id, char *publisher_id, char *reseller_ref, char *reseller_agent_id);

 
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
  char * postEntityAdvertiserCreate( char *entity_id, char *tags, char *locations, char *max_tags, char *max_locations, char *expiry_date, char *is_national, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id);

 
  /**
   * Adds/removes locations
   *
   *  @param entity_id
   *  @param gen_id
   *  @param locations_to_add
   *  @param locations_to_remove
   *  @return - the data from the api
  */
  char * postEntityAdvertiserLocation( char *entity_id, char *gen_id, char *locations_to_add, char *locations_to_remove);

 
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
  char * postEntityAdvertiserRenew( char *entity_id, char *expiry_date, char *publisher_id, char *reseller_ref, char *reseller_agent_id);

 
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
  char * postEntityAdvertiserTag( char *gen_id, char *entity_id, char *language, char *tags_to_add, char *tags_to_remove);

 
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
  char * postEntityAdvertiserUpsell( char *entity_id, char *tags, char *locations, char *extra_tags, char *extra_locations, char *is_national, char *language, char *reseller_ref, char *reseller_agent_id, char *publisher_id);

 
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
  char * getEntityAdvertisers( char *tag, char *where, char *limit, char *country, char *language);

 
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
  char * postEntityAffiliate_link( char *entity_id, char *affiliate_name, char *affiliate_link, char *affiliate_message, char *affiliate_logo);

 
  /**
   * Allows an affiliate link object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityAffiliate_link( char *entity_id, char *gen_id);

 
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
  char * postEntityBackground( char *entity_id, char *number_of_employees, char *turnover, char *net_profit, char *vat_number, char *duns_number, char *registered_company_number);

 
  /**
   * Uploads a CSV file of known format and bulk inserts into DB
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * postEntityBulkCsv( char *filedata);

 
  /**
   * Shows the current status of a bulk upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * getEntityBulkCsvStatus( char *upload_id);

 
  /**
   * Uploads a JSON file of known format and bulk inserts into DB
   *
   *  @param data
   *  @return - the data from the api
  */
  char * postEntityBulkJson( char *data);

 
  /**
   * Shows the current status of a bulk JSON upload
   *
   *  @param upload_id
   *  @return - the data from the api
  */
  char * getEntityBulkJsonStatus( char *upload_id);

 
  /**
   * Get all entities within a specified group
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * getEntityBy_groupid( char *group_id);

 
  /**
   * uncontributes a given entities supplier content and makes the entity inactive if the entity is un-usable
   *
   *  @param entity_id - The entity to pull
   *  @param supplier_masheryid - The suppliers masheryid to match
   *  @param supplier_id - The supplier id to match
   *  @param supplier_user_id - The user id to match
   *  @return - the data from the api
  */
  char * deleteEntityBy_supplier( char *entity_id, char *supplier_masheryid, char *supplier_id, char *supplier_user_id);

 
  /**
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID
   *  @return - the data from the api
  */
  char * getEntityBy_supplier_id( char *supplier_id);

 
  /**
   * Get all entiies claimed by a specific user
   *
   *  @param user_id - The unique user ID of the user with claimed entities e.g. 379236608286720
   *  @return - the data from the api
  */
  char * getEntityBy_user_id( char *user_id);

 
  /**
   * With a known entity id, an category object can be added.
   *
   *  @param entity_id
   *  @param category_id
   *  @param category_type
   *  @return - the data from the api
  */
  char * postEntityCategory( char *entity_id, char *category_id, char *category_type);

 
  /**
   * Allows a category object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityCategory( char *entity_id, char *gen_id);

 
  /**
   * Fetches the changelog documents that match the given entity_id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * getEntityChangelog( char *entity_id);

 
  /**
   * Allow an entity to be claimed by a valid user
   *
   *  @param entity_id
   *  @param claimed_user_id
   *  @param claimed_date
   *  @param claim_method
   *  @param phone_number
   *  @param referrer_url
   *  @param referrer_name
   *  @return - the data from the api
  */
  char * postEntityClaim( char *entity_id, char *claimed_user_id, char *claimed_date, char *claim_method, char *phone_number, char *referrer_url, char *referrer_name);

 
  /**
   * Allows a description object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityDescription( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @return - the data from the api
  */
  char * postEntityDescription( char *entity_id, char *headline, char *body);

 
  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityDocument( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, an document object can be added.
   *
   *  @param entity_id
   *  @param name
   *  @param filedata
   *  @return - the data from the api
  */
  char * postEntityDocument( char *entity_id, char *name, char *filedata);

 
  /**
   * With a known entity id, an email address object can be added.
   *
   *  @param entity_id
   *  @param email_address
   *  @param email_description
   *  @return - the data from the api
  */
  char * postEntityEmail( char *entity_id, char *email_address, char *email_description);

 
  /**
   * Allows a email object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityEmail( char *entity_id, char *gen_id);

 
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
  char * postEntityEmployee( char *entity_id, char *title, char *forename, char *surname, char *job_title, char *description, char *email, char *phone_number);

 
  /**
   * Allows an employee object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityEmployee( char *entity_id, char *gen_id);

 
  /**
   * Allows a fax object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityFax( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, an fax object can be added.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @return - the data from the api
  */
  char * postEntityFax( char *entity_id, char *number, char *description);

 
  /**
   * With a known entity id, a geopoint can be updated.
   *
   *  @param entity_id
   *  @param longitude
   *  @param latitude
   *  @param accuracy
   *  @return - the data from the api
  */
  char * postEntityGeopoint( char *entity_id, char *longitude, char *latitude, char *accuracy);

 
  /**
   * Allows a group object to be removed from an entities group members
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityGroup( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a group  can be added to group members.
   *
   *  @param entity_id
   *  @param group_id
   *  @return - the data from the api
  */
  char * postEntityGroup( char *entity_id, char *group_id);

 
  /**
   * Allows a image object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityImage( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a image object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param image_name
   *  @return - the data from the api
  */
  char * postEntityImage( char *entity_id, char *filedata, char *image_name);

 
  /**
   * With a known entity id and a known invoice_address ID, we can delete a specific invoice_address object from an enitity.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * deleteEntityInvoice_address( char *entity_id);

 
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
  char * postEntityInvoice_address( char *entity_id, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *address_type);

 
  /**
   * Allows a list description object to be reduced in confidence
   *
   *  @param gen_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * deleteEntityList( char *gen_id, char *entity_id);

 
  /**
   * With a known entity id, a list description object can be added.
   *
   *  @param entity_id
   *  @param headline
   *  @param body
   *  @return - the data from the api
  */
  char * postEntityList( char *entity_id, char *headline, char *body);

 
  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityLogo( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a logo object can be added.
   *
   *  @param entity_id
   *  @param filedata
   *  @param logo_name
   *  @return - the data from the api
  */
  char * postEntityLogo( char *entity_id, char *filedata, char *logo_name);

 
  /**
   * Merge two entities into one
   *
   *  @param from
   *  @param to
   *  @param override_trust - Do you want to override the trust of the 'from' entity
   *  @param uncontribute_masheryid - Do we want to uncontribute any data for a masheryid?
   *  @param uncontribute_userid - Do we want to uncontribute any data for a user_id?
   *  @param uncontribute_supplierid - Do we want to uncontribute any data for a supplier_id?
   *  @return - the data from the api
  */
  char * postEntityMerge( char *from, char *to, char *override_trust, char *uncontribute_masheryid, char *uncontribute_userid, char *uncontribute_supplierid);

 
  /**
   * Update entities that use an old category ID to a new one
   *
   *  @param from
   *  @param to
   *  @param limit
   *  @return - the data from the api
  */
  char * postEntityMigrate_category( char *from, char *to, char *limit);

 
  /**
   * With a known entity id, a name can be updated.
   *
   *  @param entity_id
   *  @param name
   *  @param formal_name
   *  @return - the data from the api
  */
  char * postEntityName( char *entity_id, char *name, char *formal_name);

 
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
  char * postEntityOpening_times( char *entity_id, char *monday, char *tuesday, char *wednesday, char *thursday, char *friday, char *saturday, char *sunday, char *closed, char *closed_public_holidays);

 
  /**
   * With a known entity id, a opening times object can be removed.
   *
   *  @param entity_id - The id of the entity to edit
   *  @return - the data from the api
  */
  char * deleteEntityOpening_times( char *entity_id);

 
  /**
   * Allows a new phone object to be added to a specified entity. A new object id will be calculated and returned to you if successful.
   *
   *  @param entity_id
   *  @param number
   *  @param description
   *  @return - the data from the api
  */
  char * postEntityPhone( char *entity_id, char *number, char *description);

 
  /**
   * Allows a phone object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityPhone( char *entity_id, char *gen_id);

 
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
  char * postEntityPostal_address( char *entity_id, char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *address_type, char *do_not_display);

 
  /**
   * Fetches the documents that match the given masheryid and supplier_id
   *
   *  @param supplier_id - The Supplier ID
   *  @return - the data from the api
  */
  char * getEntityProvisionalBy_supplier_id( char *supplier_id);

 
  /**
   * Allows a list of available revisions to be returned by its entity id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * getEntityRevisions( char *entity_id);

 
  /**
   * Allows a specific revision of an entity to be returned by entity id and a revision number
   *
   *  @param entity_id
   *  @param revision_id
   *  @return - the data from the api
  */
  char * getEntityRevisionsByRevisionID( char *entity_id, char *revision_id);

 
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
  char * getEntitySearchByboundingbox( char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *per_page, char *page, char *country, char *language);

 
  /**
   * Search for matching entities
   *
   *  @param where - Location to search for results. E.g. Dublin e.g. Dublin
   *  @param per_page - How many results per page
   *  @param page - What page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the search context (optional)
   *  @param longitude - The decimal longitude of the search context (optional)
   *  @return - the data from the api
  */
  char * getEntitySearchBylocation( char *where, char *per_page, char *page, char *country, char *language, char *latitude, char *longitude);

 
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
  char * getEntitySearchWhat( char *what, char *per_page, char *page, char *country, char *language);

 
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
  char * getEntitySearchWhatByboundingbox( char *what, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *per_page, char *page, char *country, char *language);

 
  /**
   * Search for matching entities
   *
   *  @param what - What to get results for. E.g. Plumber e.g. plumber
   *  @param where - The location to get results for. E.g. Dublin e.g. Dublin
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @param latitude - The decimal latitude of the search context (optional)
   *  @param longitude - The decimal longitude of the search context (optional)
   *  @return - the data from the api
  */
  char * getEntitySearchWhatBylocation( char *what, char *where, char *per_page, char *page, char *country, char *language, char *latitude, char *longitude);

 
  /**
   * Search for matching entities
   *
   *  @param who - Company name e.g. Starbucks
   *  @param per_page - How many results per page
   *  @param page - What page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntitySearchWho( char *who, char *per_page, char *page, char *country, char *language);

 
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
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntitySearchWhoByboundingbox( char *who, char *latitude_1, char *longitude_1, char *latitude_2, char *longitude_2, char *per_page, char *page, char *country, char *language);

 
  /**
   * Search for matching entities
   *
   *  @param who - Company Name e.g. Starbucks
   *  @param where - The location to get results for. E.g. Dublin e.g. Dublin
   *  @param per_page - Number of results returned per page
   *  @param page - Which page number to retrieve
   *  @param country - Which country to return results for. An ISO compatible country code, E.g. ie e.g. ie
   *  @param latitude - The decimal latitude of the search context (optional)
   *  @param longitude - The decimal longitude of the search context (optional)
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getEntitySearchWhoBylocation( char *who, char *where, char *per_page, char *page, char *country, char *latitude, char *longitude, char *language);

 
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
  char * postEntitySend_email( char *entity_id, char *gen_id, char *from_email, char *subject, char *content);

 
  /**
   * Allows a social media object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntitySocialmedia( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a social media object can be added.
   *
   *  @param entity_id
   *  @param type
   *  @param website_url
   *  @return - the data from the api
  */
  char * postEntitySocialmedia( char *entity_id, char *type, char *website_url);

 
  /**
   * Allows a special offer object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntitySpecial_offer( char *entity_id, char *gen_id);

 
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
  char * postEntitySpecial_offer( char *entity_id, char *title, char *description, char *terms, char *start_date, char *expiry_date, char *url, char *image_url);

 
  /**
   * With a known entity id, a status object can be updated.
   *
   *  @param entity_id
   *  @param status
   *  @param inactive_reason
   *  @param inactive_description
   *  @return - the data from the api
  */
  char * postEntityStatus( char *entity_id, char *status, char *inactive_reason, char *inactive_description);

 
  /**
   * With a known entity id, an tag object can be added.
   *
   *  @param entity_id
   *  @param tag
   *  @param language
   *  @return - the data from the api
  */
  char * postEntityTag( char *entity_id, char *tag, char *language);

 
  /**
   * Allows a tag object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityTag( char *entity_id, char *gen_id);

 
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
  char * postEntityTestimonial( char *entity_id, char *title, char *text, char *date, char *testifier_name);

 
  /**
   * Allows a testimonial object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityTestimonial( char *entity_id, char *gen_id);

 
  /**
   * Get the updates a uncontribute would perform
   *
   *  @param entity_id - The entity to pull
   *  @param object_name - The entity object to update
   *  @param supplier_id - The supplier_id to remove
   *  @param user_id - The user_id to remove
   *  @return - the data from the api
  */
  char * getEntityUncontribute( char *entity_id, char *object_name, char *supplier_id, char *user_id);

 
  /**
   * Separates an entity into two distinct entities 
   *
   *  @param entity_id
   *  @param supplier_masheryid
   *  @param supplier_id
   *  @return - the data from the api
  */
  char * postEntityUnmerge( char *entity_id, char *supplier_masheryid, char *supplier_id);

 
  /**
   * Allows a video object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityVideo( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a YouTube video object can be added.
   *
   *  @param entity_id
   *  @param embed_code
   *  @return - the data from the api
  */
  char * postEntityVideoYoutube( char *entity_id, char *embed_code);

 
  /**
   * Allows a website object to be reduced in confidence
   *
   *  @param entity_id
   *  @param gen_id
   *  @return - the data from the api
  */
  char * deleteEntityWebsite( char *entity_id, char *gen_id);

 
  /**
   * With a known entity id, a website object can be added.
   *
   *  @param entity_id
   *  @param website_url
   *  @param display_url
   *  @param website_description
   *  @return - the data from the api
  */
  char * postEntityWebsite( char *entity_id, char *website_url, char *display_url, char *website_description);

 
  /**
   * Add an entityserve document
   *
   *  @param entity_id - The id of the entity to create the entityserve event for
   *  @param country - the ISO code of the country
   *  @param event_type - The event type being recorded
   *  @return - the data from the api
  */
  char * putEntityserve( char *entity_id, char *country, char *event_type);

 
  /**
   * Remove a flatpack using a supplied flatpack_id
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @return - the data from the api
  */
  char * deleteFlatpack( char *flatpack_id);

 
  /**
   * Update/Add a flatpack
   *
   *  @param flatpack_id - this record's unique, auto-generated id - if supplied, then this is an edit, otherwise it's an add
   *  @param domainName - the domain name to serve this flatpack site on (no leading http:// or anything please)
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
   *  @param serpTitleNoWhat - The text to display in the title for where only searches
   *  @param serpDescriptionNoWhat - The text to display in the description for where only searches
   *  @param cookiePolicyUrl - The cookie policy url of the flatpack
   *  @param cookiePolicyNotice - Whether to show the cookie policy on this flatpack
   *  @param addBusinessButtonText - The text used in the 'Add your business' button
   *  @param twitterUrl - Twitter link
   *  @param facebookUrl - Facebook link
   *  @param copyright - Copyright message
   *  @param advertUpgradeActive - whether upgrade message is displayed on this Flatpack
   *  @param advertUpgradePrice - the cost of upgrading
   *  @param advertUpgradeMaxTags - the number of tags upgrading gives you
   *  @param advertUpgradeMaxLocations - the number of locations upgrading gives you
   *  @param advertUpgradeContractLength - the length of the contract (days)
   *  @param advertUpgradeRefId - a unique reference for the upgrade
   *  @param phoneReveal - record phone number reveal
   *  @param loginLinkText - the link text for the Login link
   *  @param contextLocationId - The location ID to use as the context for searches on this flatpack
   *  @param addBusinessButtonPosition - The location ID to use as the context for searches on this flatpack
   *  @param denyIndexing - Whether to noindex a flatpack
   *  @param contextRadius - allows you to set a catchment area around the contextLocationId in miles for use when displaying the activity stream module
   *  @param activityStream - allows you to set the activity to be displayed in the activity stream
   *  @param activityStreamSize - Sets the number of items to show within the activity stream.
   *  @return - the data from the api
  */
  char * postFlatpack( char *flatpack_id, char *domainName, char *stub, char *flatpackName, char *less, char *language, char *country, char *mapsType, char *mapKey, char *searchFormShowOn, char *searchFormShowKeywordsBox, char *searchFormShowLocationBox, char *searchFormKeywordsAutoComplete, char *searchFormLocationsAutoComplete, char *searchFormDefaultLocation, char *searchFormPlaceholderKeywords, char *searchFormPlaceholderLocation, char *searchFormKeywordsLabel, char *searchFormLocationLabel, char *cannedLinksHeader, char *homepageTitle, char *homepageDescription, char *homepageIntroTitle, char *homepageIntroText, char *head, char *adblock, char *bodyTop, char *bodyBottom, char *header_menu, char *header_menu_bottom, char *footer_menu, char *bdpTitle, char *bdpDescription, char *bdpAds, char *serpTitle, char *serpDescription, char *serpNumberResults, char *serpNumberAdverts, char *serpAds, char *serpTitleNoWhat, char *serpDescriptionNoWhat, char *cookiePolicyUrl, char *cookiePolicyNotice, char *addBusinessButtonText, char *twitterUrl, char *facebookUrl, char *copyright, char *advertUpgradeActive, char *advertUpgradePrice, char *advertUpgradeMaxTags, char *advertUpgradeMaxLocations, char *advertUpgradeContractLength, char *advertUpgradeRefId, char *phoneReveal, char *loginLinkText, char *contextLocationId, char *addBusinessButtonPosition, char *denyIndexing, char *contextRadius, char *activityStream, char *activityStreamSize);

 
  /**
   * Get a flatpack
   *
   *  @param flatpack_id - the unique id to search for
   *  @return - the data from the api
  */
  char * getFlatpack( char *flatpack_id);

 
  /**
   * Upload a CSS file for the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackAdminCSS( char *flatpack_id, char *filedata);

 
  /**
   * Upload an image to serve out as the large logo in the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackAdminLargeLogo( char *flatpack_id, char *filedata);

 
  /**
   * Upload an image to serve out as the small logo in the Admin for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackAdminSmallLogo( char *flatpack_id, char *filedata);

 
  /**
   * Get a flatpack using a domain name
   *
   *  @param domainName - the domain name to search for
   *  @return - the data from the api
  */
  char * getFlatpackBy_domain_name( char *domainName);

 
  /**
   * Get flatpacks that match the supplied masheryid
   *
   *  @return - the data from the api
  */
  char * getFlatpackBy_masheryid();

 
  /**
   * Clone an existing flatpack
   *
   *  @param flatpack_id - the flatpack_id to clone
   *  @param domainName - the domain of the new flatpack site (no leading http:// or anything please)
   *  @return - the data from the api
  */
  char * getFlatpackClone( char *flatpack_id, char *domainName);

 
  /**
   * Upload an icon to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackIcon( char *flatpack_id, char *filedata);

 
  /**
   * Remove a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param gen_id - the id of the canned link to remove
   *  @return - the data from the api
  */
  char * deleteFlatpackLink( char *flatpack_id, char *gen_id);

 
  /**
   * Add a canned link to an existing flatpack site.
   *
   *  @param flatpack_id - the id of the flatpack to delete
   *  @param keywords - the keywords to use in the canned search
   *  @param location - the location to use in the canned search
   *  @param linkText - the link text to be used to in the canned search link
   *  @return - the data from the api
  */
  char * postFlatpackLink( char *flatpack_id, char *keywords, char *location, char *linkText);

 
  /**
   * Upload a logo to serve out with this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackLogo( char *flatpack_id, char *filedata);

 
  /**
   * Upload a TXT file to act as the sitemap for this flatpack
   *
   *  @param flatpack_id - the id of the flatpack to update
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackSitemap( char *flatpack_id, char *filedata);

 
  /**
   * Upload a file to our asset server and return the url
   *
   *  @param filedata
   *  @return - the data from the api
  */
  char * postFlatpackUpload( char *filedata);

 
  /**
   * Returns group that matches a given group id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * getGroup( char *group_id);

 
  /**
   * Update/Add a Group
   *
   *  @param group_id
   *  @param name
   *  @param description
   *  @param url
   *  @return - the data from the api
  */
  char * postGroup( char *group_id, char *name, char *description, char *url);

 
  /**
   * Delete a group with a specified group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * deleteGroup( char *group_id);

 
  /**
   * Returns all groups
   *
   *  @return - the data from the api
  */
  char * getGroupAll();

 
  /**
   * Bulk delete entities from a specified group
   *
   *  @param group_id
   *  @param filedata
   *  @return - the data from the api
  */
  char * postGroupBulk_delete( char *group_id, char *filedata);

 
  /**
   * Bulk ingest entities into a specified group
   *
   *  @param group_id
   *  @param filedata
   *  @param category_type
   *  @return - the data from the api
  */
  char * postGroupBulk_ingest( char *group_id, char *filedata, char *category_type);

 
  /**
   * Bulk update entities with a specified group
   *
   *  @param group_id
   *  @param data
   *  @return - the data from the api
  */
  char * postGroupBulk_update( char *group_id, char *data);

 
  /**
   * Get number of claims today
   *
   *  @param from_date
   *  @param to_date
   *  @param country_id
   *  @return - the data from the api
  */
  char * getHeartbeatBy_date( char *from_date, char *to_date, char *country_id);

 
  /**
   * Get number of claims today
   *
   *  @param country
   *  @param claim_type
   *  @return - the data from the api
  */
  char * getHeartbeatTodayClaims( char *country, char *claim_type);

 
  /**
   * Process a bulk file
   *
   *  @param job_id
   *  @param filedata - A tab separated file for ingest
   *  @return - the data from the api
  */
  char * postIngest_file( char *job_id, char *filedata);

 
  /**
   * Add a ingest job to the collection
   *
   *  @param description
   *  @param category_type
   *  @return - the data from the api
  */
  char * postIngest_job( char *description, char *category_type);

 
  /**
   * Get an ingest job from the collection
   *
   *  @param job_id
   *  @return - the data from the api
  */
  char * getIngest_job( char *job_id);

 
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
  char * getIngest_logBy_job_id( char *job_id, char *success, char *errors, char *limit, char *skip);

 
  /**
   * Check the status of the Ingest queue, and potentially flush it
   *
   *  @param flush
   *  @return - the data from the api
  */
  char * getIngest_queue( char *flush);

 
  /**
   * Read a location with the supplied ID in the locations reference database.
   *
   *  @param location_id
   *  @return - the data from the api
  */
  char * getLocation( char *location_id);

 
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
  char * postLocation( char *location_id, char *type, char *country, char *language, char *name, char *formal_name, char *resolution, char *population, char *description, char *timezone, char *latitude, char *longitude, char *parent_town, char *parent_county, char *parent_province, char *parent_region, char *parent_neighbourhood, char *parent_district, char *postalcode, char *searchable_id, char *searchable_ids);

 
  /**
   * Read multiple locations with the supplied ID in the locations reference database.
   *
   *  @param location_ids
   *  @return - the data from the api
  */
  char * getLocationMultiple( char *location_ids);

 
  /**
   * Fetch the project logo, the symbol of the Wolf
   *
   *  @param a
   *  @param b
   *  @param c
   *  @param d
   *  @return - the data from the api
  */
  char * getLogo( char *a, char *b, char *c, char *d);

 
  /**
   * Fetch the project logo, the symbol of the Wolf
   *
   *  @param a
   *  @return - the data from the api
  */
  char * putLogo( char *a);

 
  /**
   * Find a category from cache or cloudant depending if it is in the cache
   *
   *  @param string - A string to search against, E.g. Plumbers
   *  @param language - An ISO compatible language code, E.g. en
   *  @return - the data from the api
  */
  char * getLookupCategory( char *string, char *language);

 
  /**
   * Find a category from a legacy ID or supplier (e.g. bill_moss)
   *
   *  @param id
   *  @param type
   *  @return - the data from the api
  */
  char * getLookupLegacyCategory( char *id, char *type);

 
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
  char * getLookupLocation( char *string, char *language, char *country, char *latitude, char *longitude);

 
  /**
   * Find all matches by phone number and then return all matches that also match company name and location. Default location_strictness is defined in Km and the default is set to 0.2 (200m)
   *
   *  @param phone
   *  @param company_name
   *  @param latitude
   *  @param longitude
   *  @param postcode
   *  @param country
   *  @param name_strictness
   *  @param location_strictness
   *  @return - the data from the api
  */
  char * getMatchByphone( char *phone, char *company_name, char *latitude, char *longitude, char *postcode, char *country, char *name_strictness, char *location_strictness);

 
  /**
   * Find all matches by phone number, returning up to 10 matches
   *
   *  @param phone
   *  @param country
   *  @param exclude - Entity ID to exclude from the results
   *  @return - the data from the api
  */
  char * getMatchByphone2( char *phone, char *country, char *exclude);

 
  /**
   * Perform a match on the two supplied entities, returning the outcome and showing our working
   *
   *  @param primary_entity_id
   *  @param secondary_entity_id
   *  @param return_entities - Should we return the entity documents
   *  @return - the data from the api
  */
  char * getMatchOftheday( char *primary_entity_id, char *secondary_entity_id, char *return_entities);

 
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
  char * postMessage( char *message_id, char *ses_id, char *from_user_id, char *from_email, char *to_entity_id, char *to_email, char *subject, char *body, char *bounced);

 
  /**
   * Fetching a message
   *
   *  @param message_id - The message id to pull the message for
   *  @return - the data from the api
  */
  char * getMessage( char *message_id);

 
  /**
   * Fetching messages by ses_id
   *
   *  @param ses_id - The amazon id to pull the message for
   *  @return - the data from the api
  */
  char * getMessageBy_ses_id( char *ses_id);

 
  /**
   * With a known entity id, a private object can be added.
   *
   *  @param entity_id - The entity to associate the private object with
   *  @param data - The data to store
   *  @return - the data from the api
  */
  char * putPrivate_object( char *entity_id, char *data);

 
  /**
   * Allows a private object to be removed
   *
   *  @param private_object_id - The id of the private object to remove
   *  @return - the data from the api
  */
  char * deletePrivate_object( char *private_object_id);

 
  /**
   * Allows a private object to be returned based on the entity_id and masheryid
   *
   *  @param entity_id - The entity associated with the private object
   *  @return - the data from the api
  */
  char * getPrivate_objectAll( char *entity_id);

 
  /**
   * Perform the whole PTB process on the supplied entity
   *
   *  @param entity_id
   *  @param destructive
   *  @return - the data from the api
  */
  char * getPtbAll( char *entity_id, char *destructive);

 
  /**
   * Report on what happened to specific entity_id
   *
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param entity_id - the entity to research
   *  @return - the data from the api
  */
  char * getPtbLog( char *year, char *month, char *entity_id);

 
  /**
   * Process an entity with a specific PTB module
   *
   *  @param entity_id
   *  @param module
   *  @param destructive
   *  @return - the data from the api
  */
  char * getPtbModule( char *entity_id, char *module, char *destructive);

 
  /**
   * Report on the run-rate of the Paint the Bridge System
   *
   *  @param country - the country to get the runrate for
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param day - the day to examine
   *  @return - the data from the api
  */
  char * getPtbRunrate( char *country, char *year, char *month, char *day);

 
  /**
   * Report on the value being added by Paint The Bridge
   *
   *  @param country - the country to get the runrate for
   *  @param year - the year to examine
   *  @param month - the month to examine
   *  @param day - the day to examine
   *  @return - the data from the api
  */
  char * getPtbValueadded( char *country, char *year, char *month, char *day);

 
  /**
   * Returns publisher that matches a given publisher id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * getPublisher( char *publisher_id);

 
  /**
   * Delete a publisher with a specified publisher_id
   *
   *  @param publisher_id
   *  @return - the data from the api
  */
  char * deletePublisher( char *publisher_id);

 
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
  char * postPublisher( char *publisher_id, char *country, char *name, char *description, char *active);

 
  /**
   * Returns publisher that matches a given publisher id
   *
   *  @param country
   *  @return - the data from the api
  */
  char * getPublisherByCountry( char *country);

 
  /**
   * Returns publishers that are available for a given entity_id.
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * getPublisherByEntityId( char *entity_id);

 
  /**
   * Create a queue item
   *
   *  @param queue_name
   *  @param data
   *  @return - the data from the api
  */
  char * putQueue( char *queue_name, char *data);

 
  /**
   * With a known queue id, a queue item can be removed.
   *
   *  @param queue_id
   *  @return - the data from the api
  */
  char * deleteQueue( char *queue_id);

 
  /**
   * Retrieve queue items.
   *
   *  @param limit
   *  @param queue_name
   *  @return - the data from the api
  */
  char * getQueue( char *limit, char *queue_name);

 
  /**
   * Add an error to a queue item
   *
   *  @param queue_id
   *  @param error
   *  @return - the data from the api
  */
  char * postQueueError( char *queue_id, char *error);

 
  /**
   * Find a queue item by its type and id
   *
   *  @param type
   *  @param id
   *  @return - the data from the api
  */
  char * getQueueSearch( char *type, char *id);

 
  /**
   * Unlock queue items.
   *
   *  @param queue_name
   *  @param seconds
   *  @return - the data from the api
  */
  char * postQueueUnlock( char *queue_name, char *seconds);

 
  /**
   * Return a sales log by id
   *
   *  @param sales_log_id - The sales log id to pull
   *  @return - the data from the api
  */
  char * getSales_log( char *sales_log_id);

 
  /**
   * Return a sales log by id
   *
   *  @param from_date
   *  @param country
   *  @param action_type
   *  @return - the data from the api
  */
  char * getSales_logBy_countryBy_date( char *from_date, char *country, char *action_type);

 
  /**
   * Return a sales log by id
   *
   *  @param from_date
   *  @param to_date
   *  @return - the data from the api
  */
  char * getSales_logBy_date( char *from_date, char *to_date);

 
  /**
   * Log a sale
   *
   *  @param entity_id - The entity the sale was made against
   *  @param country - The country code the sales log orginated
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
  char * postSales_logEntity( char *entity_id, char *country, char *action_type, char *publisher_id, char *mashery_id, char *reseller_ref, char *reseller_agent_id, char *max_tags, char *max_locations, char *extra_tags, char *extra_locations, char *expiry_date);

 
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
  char * postSales_logSyndication( char *action_type, char *syndication_type, char *publisher_id, char *expiry_date, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country, char *reseller_masheryid);

 
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
   *  @return - the data from the api
  */
  char * postSignal( char *entity_id, char *country, char *gen_id, char *signal_type, char *data_type, char *inactive_reason, char *inactive_description);

 
  /**
   * Get the number of times an entity has been served out as an advert or on serps/bdp pages
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @param month - The month to report on
   *  @return - the data from the api
  */
  char * getStatsEntityBy_date( char *entity_id, char *year, char *month);

 
  /**
   * Get the stats on an entity in a given year
   *
   *  @param entity_id - A valid entity_id e.g. 379236608286720
   *  @param year - The year to report on
   *  @return - the data from the api
  */
  char * getStatsEntityBy_year( char *entity_id, char *year);

 
  /**
   * Confirms that the API is active, and returns the current version number
   *
   *  @return - the data from the api
  */
  char * getStatus();

 
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
   *  @return - the data from the api
  */
  char * postSyndicationCreate( char *syndication_type, char *publisher_id, char *expiry_date, char *entity_id, char *group_id, char *seed_masheryid, char *supplier_masheryid, char *country);

 
  /**
   * When we get a syndication update make a record of it
   *
   *  @param entity_id - The entity to pull
   *  @param publisher_id - The publisher this log entry refers to
   *  @param action - The log type
   *  @param success - If the syndication was successful
   *  @param message - An optional message e.g. submitted to the syndication partner
   *  @param syndicated_id - The entity as known to the publisher
   *  @return - the data from the api
  */
  char * postSyndication_log( char *entity_id, char *publisher_id, char *action, char *success, char *message, char *syndicated_id);

 
  /**
   * Get all syndication log entries for a given entity id
   *
   *  @param entity_id
   *  @return - the data from the api
  */
  char * getSyndication_logBy_entity_id( char *entity_id);

 
  /**
   * Provides a tokenised URL to redirect a user so they can add an entity to Central Index
   *
   *  @param language - The language to use to render the add path e.g. en
   *  @param portal_name - The name of the website that data is to be added on e.g. YourLocal
   *  @param country - The country of the entity to be added e.g. gb
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * getTokenAdd( char *language, char *portal_name, char *country, char *flatpack_id);

 
  /**
   * Provides a tokenised URL to redirect a user to claim an entity on Central Index
   *
   *  @param entity_id - Entity ID to be claimed e.g. 380348266819584
   *  @param language - The language to use to render the claim path e.g. en
   *  @param portal_name - The name of the website that entity is being claimed on e.g. YourLocal
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * getTokenClaim( char *entity_id, char *language, char *portal_name, char *flatpack_id);

 
  /**
   * Allows us to identify the user, entity and element from an encoded endpoint URL's token
   *
   *  @param token
   *  @return - the data from the api
  */
  char * getTokenDecode( char *token);

 
  /**
   * Fetch token for edit path
   *
   *  @param entity_id - The id of the entity being upgraded
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @param edit_page - the page in the edit path that the user should land on
   *  @return - the data from the api
  */
  char * getTokenEdit( char *entity_id, char *language, char *flatpack_id, char *edit_page);

 
  /**
   * Fetch token for login path
   *
   *  @param portal_name - The name of the application that has initiated the login process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * getTokenLogin( char *portal_name, char *language, char *flatpack_id);

 
  /**
   * Fetch token for messaging path
   *
   *  @param entity_id - The id of the entity being messaged
   *  @param portal_name - The name of the application that has initiated the email process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * getTokenMessage( char *entity_id, char *portal_name, char *language, char *flatpack_id);

 
  /**
   * Provides a tokenised URL that allows a user to report incorrect entity information
   *
   *  @param entity_id - The unique Entity ID e.g. 379236608286720
   *  @param portal_name - The name of the portal that the user is coming from e.g. YourLocal
   *  @param language - The language to use to render the report path
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * getTokenReport( char *entity_id, char *portal_name, char *language, char *flatpack_id);

 
  /**
   * Fetch token for update path
   *
   *  @param entity_id - The id of the entity being upgraded
   *  @param portal_name - The name of the application that has initiated the login process, example: 'Your Local'
   *  @param language - The language for the app
   *  @param price - The price of the advert in the entities native currency
   *  @param max_tags - The number of tags attached to the advert
   *  @param max_locations - The number of locations attached to the advert
   *  @param contract_length - The number of days from the initial sale date that the contract is valid for
   *  @param ref_id - The campaign or reference id
   *  @param flatpack_id - The id of the flatpack site where the request originated
   *  @return - the data from the api
  */
  char * getTokenUpgrade( char *entity_id, char *portal_name, char *language, char *price, char *max_tags, char *max_locations, char *contract_length, char *ref_id, char *flatpack_id);

 
  /**
   * The JaroWinklerDistance between two entities postal address objects
   *
   *  @param first_entity_id - The entity id of the first entity to be used in the postal address difference
   *  @param second_entity_id - The entity id of the second entity to be used in the postal address difference
   *  @return - the data from the api
  */
  char * getToolsAddressdiff( char *first_entity_id, char *second_entity_id);

 
  /**
   * Use this call to get information (in HTML or JSON) about the data structure of given entity object (e.g. a phone number or an address)
   *
   *  @param object - The API call documentation is required for
   *  @param format - The format of the returned data eg. JSON or HTML
   *  @return - the data from the api
  */
  char * getToolsDocs( char *object, char *format);

 
  /**
   * Format an address according to the rules of the country supplied
   *
   *  @param address - The address to format
   *  @param country - The country where the address is based
   *  @return - the data from the api
  */
  char * getToolsFormatAddress( char *address, char *country);

 
  /**
   * Format a phone number according to the rules of the country supplied
   *
   *  @param number - The telephone number to format
   *  @param country - The country where the telephone number is based
   *  @return - the data from the api
  */
  char * getToolsFormatPhone( char *number, char *country);

 
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
  char * getToolsGeocode( char *building_number, char *address1, char *address2, char *address3, char *district, char *town, char *county, char *province, char *postcode, char *country);

 
  /**
   * Given a spreadsheet id add a row
   *
   *  @param spreadsheet_key - The key of the spreadsheet to edit
   *  @param data - A comma separated list to add as cells
   *  @return - the data from the api
  */
  char * postToolsGooglesheetAdd_row( char *spreadsheet_key, char *data);

 
  /**
   * Given some image data we can resize and upload the images
   *
   *  @param filedata - The image data to upload and resize
   *  @param type - The type of image to upload and resize
   *  @return - the data from the api
  */
  char * postToolsImage( char *filedata, char *type);

 
  /**
   * Generate JSON in the format to generate Mashery's IODocs
   *
   *  @param mode - The HTTP method of the API call to document. e.g. GET
   *  @param path - The path of the API call to document e.g, /entity
   *  @param endpoint - The Mashery 'endpoint' to prefix to our API paths e.g. v1
   *  @param doctype - Mashery has two forms of JSON to describe API methods; one on github, the other on its customer dashboard
   *  @return - the data from the api
  */
  char * getToolsIodocs( char *mode, char *path, char *endpoint, char *doctype);

 
  /**
   * compile the supplied less with the standard Bootstrap less into a CSS file
   *
   *  @param less - The LESS code to compile
   *  @return - the data from the api
  */
  char * getToolsLess( char *less);

 
  /**
   * Ring the person and verify their account
   *
   *  @param to - The phone number to verify
   *  @param from - The phone number to call from
   *  @param pin - The pin to verify the phone number with
   *  @param twilio_voice - The language to read the verification in
   *  @return - the data from the api
  */
  char * getToolsPhonecallVerify( char *to, char *from, char *pin, char *twilio_voice);

 
  /**
   * Return the phonetic representation of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsPhonetic( char *text);

 
  /**
   * Attempt to process a phone number, removing anything which is not a digit
   *
   *  @param number
   *  @return - the data from the api
  */
  char * getToolsProcess_phone( char *number);

 
  /**
   * Fully process a string. This includes removing punctuation, stops words and stemming a string. Also returns the phonetic representation of this string.
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsProcess_string( char *text);

 
  /**
   * Force refresh of search indexes
   *
   *  @return - the data from the api
  */
  char * getToolsReindex();

 
  /**
   * replace some text parameters with some entity details
   *
   *  @param entity_id - The entity to pull for replacements
   *  @param string - The string full of parameters
   *  @return - the data from the api
  */
  char * getToolsReplace( char *entity_id, char *string);

 
  /**
   * Check to see if a supplied email address is valid
   *
   *  @param from - The phone number from which the SMS orginates
   *  @param to - The phone number to which the SMS is to be sent
   *  @param message - The message to be sent in the SMS
   *  @return - the data from the api
  */
  char * getToolsSendsms( char *from, char *to, char *message);

 
  /**
   * Spider a single url looking for key facts
   *
   *  @param url
   *  @param pages
   *  @param country
   *  @return - the data from the api
  */
  char * getToolsSpider( char *url, char *pages, char *country);

 
  /**
   * Returns a stemmed version of a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsStem( char *text);

 
  /**
   * Removes stopwords from a string
   *
   *  @param text
   *  @return - the data from the api
  */
  char * getToolsStopwords( char *text);

 
  /**
   * Fetch the entity and convert it to DnB TSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * getToolsSyndicateDnb( char *entity_id);

 
  /**
   * Fetch the entity and convert it to TomTom XML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * getToolsSyndicateGoogle( char *entity_id);

 
  /**
   * Fetch the entity and convert it to Google KML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * getToolsSyndicateKml( char *entity_id);

 
  /**
   * Fetch the entity and convert it to Nokia CSV format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * getToolsSyndicateNokia( char *entity_id);

 
  /**
   * Fetch the entity and convert it to TomTom XML format
   *
   *  @param entity_id - The entity_id to fetch
   *  @return - the data from the api
  */
  char * getToolsSyndicateTomtom( char *entity_id);

 
  /**
   * Check to see if a supplied email address is valid
   *
   *  @param email_address - The email address to validate
   *  @return - the data from the api
  */
  char * getToolsValidate_email( char *email_address);

 
  /**
   * Calls a number to make sure it is active
   *
   *  @param phone_number - The phone number to validate
   *  @param country - The country code of the phone number to be validated
   *  @return - the data from the api
  */
  char * getToolsValidate_phone( char *phone_number, char *country);

 
  /**
   * Deleting a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * deleteTraction( char *traction_id);

 
  /**
   * Fetching a traction
   *
   *  @param traction_id
   *  @return - the data from the api
  */
  char * getTraction( char *traction_id);

 
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
  char * postTraction( char *traction_id, char *trigger_type, char *action_type, char *country, char *email_addresses, char *title, char *body, char *api_method, char *api_url, char *api_params, char *active, char *reseller_masheryid, char *publisher_masheryid, char *description);

 
  /**
   * Fetching active tractions
   *
   *  @return - the data from the api
  */
  char * getTractionActive();

 
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
  char * putTransaction( char *entity_id, char *user_id, char *basket_total, char *basket, char *currency, char *notes);

 
  /**
   * Given a transaction_id retrieve information on it
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * getTransaction( char *transaction_id);

 
  /**
   * Set a transactions status to authorised
   *
   *  @param transaction_id
   *  @param paypal_getexpresscheckoutdetails
   *  @return - the data from the api
  */
  char * postTransactionAuthorised( char *transaction_id, char *paypal_getexpresscheckoutdetails);

 
  /**
   * Given a transaction_id retrieve information on it
   *
   *  @param paypal_transaction_id
   *  @return - the data from the api
  */
  char * getTransactionBy_paypal_transaction_id( char *paypal_transaction_id);

 
  /**
   * Set a transactions status to cancelled
   *
   *  @param transaction_id
   *  @return - the data from the api
  */
  char * postTransactionCancelled( char *transaction_id);

 
  /**
   * Set a transactions status to complete
   *
   *  @param transaction_id
   *  @param paypal_doexpresscheckoutpayment
   *  @param user_id
   *  @param entity_id
   *  @return - the data from the api
  */
  char * postTransactionComplete( char *transaction_id, char *paypal_doexpresscheckoutpayment, char *user_id, char *entity_id);

 
  /**
   * Set a transactions status to inprogess
   *
   *  @param transaction_id
   *  @param paypal_setexpresscheckout
   *  @return - the data from the api
  */
  char * postTransactionInprogress( char *transaction_id, char *paypal_setexpresscheckout);

 
  /**
   * Update user based on email address or social_network/social_network_id
   *
   *  @param email
   *  @param user_id
   *  @param first_name
   *  @param last_name
   *  @param active
   *  @param trust
   *  @param creation_date
   *  @param user_type
   *  @param social_network
   *  @param social_network_id
   *  @param reseller_admin_masheryid
   *  @param group_id
   *  @param admin_upgrader
   *  @return - the data from the api
  */
  char * postUser( char *email, char *user_id, char *first_name, char *last_name, char *active, char *trust, char *creation_date, char *user_type, char *social_network, char *social_network_id, char *reseller_admin_masheryid, char *group_id, char *admin_upgrader);

 
  /**
   * With a unique ID address an user can be retrieved
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * getUser( char *user_id);

 
  /**
   * With a unique email address an user can be retrieved
   *
   *  @param email
   *  @return - the data from the api
  */
  char * getUserBy_email( char *email);

 
  /**
   * Returns all the users that match the supplied group_id
   *
   *  @param group_id
   *  @return - the data from the api
  */
  char * getUserBy_groupid( char *group_id);

 
  /**
   * Returns all the users that match the supplied reseller_admin_masheryid
   *
   *  @param reseller_admin_masheryid
   *  @return - the data from the api
  */
  char * getUserBy_reseller_admin_masheryid( char *reseller_admin_masheryid);

 
  /**
   * With a unique ID address an user can be retrieved
   *
   *  @param name
   *  @param id
   *  @return - the data from the api
  */
  char * getUserBy_social_media( char *name, char *id);

 
  /**
   * Removes group_admin privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * postUserGroup_admin_remove( char *user_id);

 
  /**
   * Removes reseller privileges from a specified user
   *
   *  @param user_id
   *  @return - the data from the api
  */
  char * postUserReseller_remove( char *user_id);

 
  /**
   * Deletes a specific social network from a user
   *
   *  @param user_id
   *  @param social_network
   *  @return - the data from the api
  */
  char * deleteUserSocial_network( char *user_id, char *social_network);

 
  /**
   * Shows what would be emitted by a view, given a document
   *
   *  @param database - the database being worked on e.g. entities
   *  @param designdoc - the design document containing the view e.g. _design/report
   *  @param view - the name of the view to be tested e.g. bydate
   *  @param doc - the JSON document to be analysed e.g. {}
   *  @return - the data from the api
  */
  char * getViewhelper( char *database, char *designdoc, char *view, char *doc);

 



