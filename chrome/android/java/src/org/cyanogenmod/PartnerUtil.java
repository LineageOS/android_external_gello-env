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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Simple utility class for centralizing resusable methods
 */
public class PartnerUtil {

    public static final String SYSPROP_USER_AGENT_OVERRIDE = "ro.browser.user_agent_override";

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

}
