package org.codeaurora.swe.partnerbrowsercustomizations;

/**
 * Contains the data contract for reading from a partner customizations provider.
 */
public class PartnerCustomizationsContract {

    /*
     * The home pages table, which holds the partner bookmarks.
     */
    public static final class HomePages {

        /**
         * This utility class cannot be instantiated.
         */
        private HomePages() {}

        /**
         * The content:// style URI for this table
         */
        public static final Uri CONTENT_URI = Uri.withAppendedPath(AUTHORITY_URI, "homepage");

        /**
         * The MIME type of a {@link #CONTENT_URI} of a single bookmark.
         */
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/partnerhomepage";

        /*** The following column names match the JSON keys! ***/

        /**
         * The unique ID for a row.
         * <p>Type: INTEGER (long)</p>
         */
        public static final String ID = "_id"; //0

        /**
         * This is the name of the page
         * <p>Type: STRING</p>
         */
        public static final String NAME = "name"; //1

        /**
         * The unique keyword identifying the page
         * <p>Type: STRING</p>
         */
        public static final String KEYWORD = "keyword"; //2

        /**
         * The templated url for the home page
         * <p>Type: STRING</p>
         */
        public static final String HOMEPAGE_URL = "homePageUrl"; //3

        /**
         * The url for the site's favicon
         * <p>Type: STRING</p>
         */
        public static final String FAVICON_URL = "faviconUrl"; //4

        /**
         * The sysprop key for fetching the runtime partner code.
         * <p>Type: STRING</p>
         *
         * This gets automatically prefixed with "ro.browser."
         *
         * e.g. if you put "partnerCodeKey": "herpderp" the system will read
         * the value of 'getprop ro.browser.herpderp" and use that as the partner
         * code for the URL.
         */
        public static final String PARTNER_CODE_KEY = "partnerCodeKey"; //5

        /**
         * The default code in case the device didn't specify a sysprop code
         * <p>Type: STRING</p>
         */
        public static final String DEFAULT_PARTNER_CODE = "defaultPartnerCode"; //6

        /**
         * The default record...there can only be one set to 1!
         * <p>Type: INTEGER</p>
         */
        public static final String DEFAULT = "default"; //7

        /**
         * This is a local favicon on the disk
         * <p>Type: BLOB</p>
         */
        public static final String FAVICON = "favicon"; //8

        /**
         * This is a local thumbnail on the disk
         * <p>Type: BLOB</p>
         */
        public static final String TOUCHICON = "touchicon"; //9

    }
}
