package org.chromium.chrome.browser.partnercustomizations;

/**
 * Contains the data contract for reading from a partner customizations provider.
 */
public class PartnerCustomizationsContract {

    /*
     * The columns and info for reading homepage info from a partner browser customizations
     * ContentProvider.
     */
    public class HomePages {
        /**
         * The total count of columns in the homepages table.
         */
        public static final int COLUMN_COUNT = 1;

        /**
         * The column containing the URL of the homepage.
         */
        public static final int HOMEPAGE_URL = 0;

        public static final int IS_DEFAULT = -1;
    }
}
