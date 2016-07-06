/*
* Copyright (C) 2016 The CyanogenMod Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

package org.cyanogenmod;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Locale;


/**
 * Simple utility class for centralizing resusable methods
 */
public class PartnerUtil {

    private static final String TAG = "CMUtil";
    public static final boolean D = true;
    public static final String SYSPROP_USER_AGENT_OVERRIDE = "ro.browser.user_agent_override";
    private static final String AUSTRAILIA_COUNTRY_CODE = "AU";
    private static final String BING_HOSTNAME = "bing.com";

    private static final HashMap<String, String> sLocaleToSearchEngineAndHomePageMap =
            new HashMap<String, String>() {
        {
            put(AUSTRAILIA_COUNTRY_CODE, BING_HOSTNAME);
            put(Locale.UK.getCountry(), BING_HOSTNAME);
            put(Locale.US.getCountry(), BING_HOSTNAME);
            put(Locale.FRANCE.getCountry(), BING_HOSTNAME);
            put(Locale.GERMANY.getCountry(), BING_HOSTNAME);
            put(Locale.JAPAN.getCountry(), BING_HOSTNAME);
            put(Locale.CANADA.getCountry(), BING_HOSTNAME);
        }
    };

    /**
     * This will fetch a system property value if it exists
     *
     * @param key {@link String}
     *
     * @return {@link String}
     *
     * @throws java.io.IOException {@link java.io.IOException} on failure to read process output
     * stream
     */
    public static String getSystemProperty(String key) throws IOException {
        Process process = Runtime.getRuntime().exec("getprop " + key);
        BufferedReader bufferedReader = new BufferedReader(
            new InputStreamReader(process.getInputStream()));
        StringBuilder outputBuilder = new StringBuilder();
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            outputBuilder.append(line).append("\n");
        }
        return outputBuilder.toString().trim();
    }

    /**
     * Get the default keyword for the Locale
     *
     * @param context {@link android.content.Context}
     *
     * @return {@link String}
     */
    public static String getDefaultSearchAndHomePagePartnerKeyword(Context context) {
        Locale locale = context.getResources().getConfiguration().locale;
        if (locale != null && locale.getCountry() != null &&
                sLocaleToSearchEngineAndHomePageMap.containsKey(locale.getCountry())) {
            return sLocaleToSearchEngineAndHomePageMap.get(locale.getCountry());
        }
        return null;
    }

    /**
     * Get the value of the system property for user agent override
     *
     * @return {@link java.lang.String} User Agent String Template
     */
    public static String getSyspropUserAgentOverride() {
        try {
            return getSystemProperty(SYSPROP_USER_AGENT_OVERRIDE);
        } catch (IOException e) {
            return null;
        }
    }

    /**
     * Read a stored asset
     *
     * @param ctx {@link android.content.Context} active context
     * @param resid {@link java.lang.Integer} asset id
     *
     * @return {@link java.lang.String} data stored in asset
     */
    public static String getRawDataFromAsset(Context ctx, int resid) {
        return readContents(ctx.getResources().openRawResource(resid));
    }

    /*
     * Reads the input stream into a string
     */
    private static String readContents(InputStream is) {
        if (is == null) {
            return null;
        }
        String line = null;
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();
        try {
            line = reader.readLine();
            while (line != null) {
                sb.append(line + "\n");
                line = reader.readLine();
            }
        } catch (IOException e) {
            Log.e(TAG, "Failed to read asset!", e);
        } finally {
            try {
                is.close();
            } catch (Exception e) {
                Log.e(TAG, "Failed to read asset!", e);
            }
        }
        return sb.toString();
    }

}
