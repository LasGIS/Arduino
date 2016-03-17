/**
 * @(#)ResourceLoader.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.MissingResourceException;
import java.util.Properties;
import java.util.Stack;
import java.util.Vector;

import org.slf4j.LoggerFactory;

/**
 * The class for loading text resources from file.
 * @author VLaskin
 * @version 1.0
 */
public final class ResourceLoader {

    /** Avoid to create the instance of class. */
    private ResourceLoader() {
    }

    /** Уровень вложенности. */
    private static final int DEPTH = 4;

    /** признак чтения системных свойств. */
    public static final boolean EXTEND_SYS_PROPERTIES = true;

    /** Global Properties. */
    private static Properties properties = loadProperties();

    /**
     * Вернуть уже загруженные глобальные свойства.
     * @return глобальные свойства
     */
    public static Properties getProperties() {
        return properties;
    }

    /**
     * Loads properties.
     * @return properties
     */
    private static Properties loadProperties() {

        Properties props;
        if (EXTEND_SYS_PROPERTIES) {
            props = new Properties(System.getProperties());
        } else {
            props = new Properties();
        }
        for (String resource : ResourceStrings.RESOURCES) {
            loadProperties(props, resource);
        }
        checkIntegrity(props);
        checkDepth(props);
        checkRecursion(props);
        return props;
    }

    /**
     * Проверка на существование набора свойств.
     * @param props набор свойств
     */
    private static void checkIntegrity(final Properties props) {
        final List<String> stack = new ArrayList<>();
        final Enumeration keys = props.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            if (!isResolved(props, key)) {
                stack.add(key);
            }
        }
        if (stack.size() > 0) {
            throw new MissingResourceException(
                "The definition(s) can't be recognized: " + stack.toString(),
                "",
                ""
            );
        }
    }

    /**
     * Проверка рекурсии.
     * @param props набор свойств
     */
    private static void checkRecursion(final Properties props) {
        final List<String> stack = new ArrayList<>();
        final Enumeration keys = props.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            if (hasRecursion(props, key)) {
                stack.add(key);
            }
        }
        if (stack.size() > 0) {
            throw new MissingResourceException(
                "Infinite loop for the definition(s): " + stack.toString(), "", ""
            );
        }
    }

    /**
     *
     * @param props набор свойств
     */
    private static void checkDepth(final Properties props) {
        final List<String> stack = new ArrayList<>();
        final Enumeration keys = props.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            if (exceedsDepth(props, key)) {
                stack.add(key);
            }
        }
        if (stack.size() > 0) {
            throw new MissingResourceException(
                "Depth exceeding for the definition(s): " + stack.toString(), "", ""
            );
        }

    }

    /**
     * Проверка на существование уже загруженных свойств.
     * @param def уже загруженные свойства
     * @param over вновь загружаемые свойства
     */
    private static void checkOverriding(final Properties def, final Properties over) {
        final Enumeration keys = def.keys();
        final List<String> stack = new ArrayList<>();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            if (over.containsKey(key)) {
                if (stack.size() < 4) {
                    stack.add(key);
                } else {
                    stack.add("...");
                    break;
                }
            }
        }
        if (stack.size() > 0) {
            throw new MissingResourceException(
                "Overridden definitions: " + stack.toString(),
                "",
                ""
            );
        }

    }

    /**
     * Читаем свойства в объект Properties def из файла под именем "resName".
     * @param def Properties объект, в который закачиваются свойства
     * @param resName имя файла со свойствами
     */
    public static void loadProperties(final Properties def, final String resName) {
        final ClassLoader ldr = ResourceLoader.class.getClassLoader();
        final InputStream in = ldr.getResourceAsStream(resName);
        final Properties props = new Properties();
        try {
            props.load(in);
            in.close();
        } catch (final Exception e) {
            throw new MissingResourceException(
                "Can't find bundle for " + resName, "", ""
            );
        }
        checkOverriding(def, props);
        final Enumeration keys = props.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            def.setProperty(key, props.getProperty(key));
        }
    }

    /**
     * Function returns value of given property from file of properties.
     * @param key Property name
     * @return Property value
     */
    public static String getResource(final String key) {
        final String res = replaceProperties(properties.getProperty(key));
        if (res != null) {
            return res;
        } else {
            throw new MissingResourceException(
                "Can't find resources for bundle: " + key,
                ResourceLoader.class.getClass().getName(),
                key
            );
        }
    }

    /**
     * Function returns value of given property from file of properties.
     * @param key Property name
     * @return Property value
     */
    public static Integer getInteger(final String key) {
        return Integer.valueOf(getResource(key));
    }

    /**
     * Function returns value of given property from file of properties or null if property is not found.
     * @param key Property key
     * @return Property value
     */
    public static String getResourceOrNull(final String key) {
        try {
            return getResource(key);
        } catch (final MissingResourceException ex) {
            return null;
        }
    } // getResourceOrNull(String): String

    /**
     * Replaces <code>${xxx}</code> style constructions in the given value
     * with the string value of the corresponding data types.
     *
     * @param value The string to be scanned for property references.
     *              May be <code>null</code>, in which case this
     *              method returns immediately with no effect.
     * @return the original string with the properties replaced, or
     *         <code>null</code> if the original string is <code>null</code>.
     */
    private static String replaceProperties(final String value) {

        if (value == null) {
            return null;
        }

        final Vector<String> fragments = new Vector<>();
        final Vector<String> refs = new Vector<>();

        parsePropertyString(value, fragments, refs);

        final StringBuilder sb = new StringBuilder();
        final Enumeration i = fragments.elements();
        final Enumeration j = refs.elements();

        while (i.hasMoreElements()) {
            String fragment = (String) i.nextElement();
            if (fragment == null) {
                final String prop = (String) j.nextElement();
                Object replacement;
                replacement = getResourceOrNull(prop);
                if (replacement == null) {
                    LoggerFactory.getLogger(ResourceLoader.class).debug(
                        "Property ${" + prop + "} has not been set"
                    );
                }
                fragment = (
                    replacement != null
                    ? replacement.toString()
                    : "${" + prop + "}"
                );
            }
            sb.append(fragment);
        }

        return sb.toString();
    }

    /**
     * Parses a string containing <code>${xxx}</code> style property
     * references into two lists. The first list is a collection
     * of text fragments, while the other is a set of string property names.
     * <code>null</code> entries in the first list indicate a property
     * reference from the second list.
     *
     * It can be overridden with a more efficient or customized version.
     *
     * @param value     Text to parse. Must not be <code>null</code>.
     * @param fragments List to add text fragments to.
     *                  Must not be <code>null</code>.
     * @param refs List to add property names to.
     *                     Must not be <code>null</code>.
     */
    private static void parsePropertyString(
        final String value, final Vector<String> fragments, final Vector<String> refs
    ) {
        int prev = 0;
        int pos;
        while ((pos = value.indexOf("$", prev)) >= 0) {
            if (pos > 0) {
                fragments.addElement(value.substring(prev, pos));
            }
            if (pos == (value.length() - 1)) {
                fragments.addElement("$");
                prev = pos + 1;
            } else if (value.charAt(pos + 1) != '{') {
                if (value.charAt(pos + 1) == '$') {
                    fragments.addElement("$");
                    prev = pos + 2;
                } else {
                    fragments.addElement(value.substring(pos, pos + 2));
                    prev = pos + 2;
                }
            } else {
                //property found, extract its name or bail on a typo
                final int endName = value.indexOf('}', pos);
                if (endName < 0) {
                    throw new MissingResourceException(
                        "Syntax error in property: ",
                        ResourceLoader.class.getName(),
                        value
                    );
                }
                final String propertyName = value.substring(pos + 2, endName);
                fragments.addElement(null);
                refs.addElement(propertyName);
                prev = endName + 1;
            }
        }
        //no more $ signs found
        //if there is any tail to the file, append it
        if (prev < value.length()) {
            fragments.addElement(value.substring(prev));
        }
    }

    /**
     *
     * @param msg
     */
//    private static void debug(String msg) {
//        System.out.println(msg);
//    }
//
    /**
     * Проверка рекурсии.
     * @param props набор свойств
     * @param key Property name
     * @return признак установленной рекурсии
     */
    private static boolean hasRecursion(final Properties props, final String key) {
        final Stack<String> stack = new Stack<>();
        return hasRecursion(props, stack, key);
    }

    /**
     * .
     * @param props набор свойств
     * @param key Property name
     * @return .
     */
    private static boolean exceedsDepth(final Properties props, final String key) {
        final Stack<String> stack = new Stack<>();
        return exceedsDepth(props, stack, key);
    }

    /**
     *
     * @param props набор свойств
     * @param key Property name
     * @return признак установленной рекурсии
     */
    private static boolean isResolved(final Properties props, final String key) {
        final Vector<String> fragments = new Vector<>();
        final Vector<String> refs = new Vector<>();
        final String val = props.getProperty(key);
        boolean res = true;

        parsePropertyString(val, fragments, refs);
        final Enumeration j = refs.elements();
        while (res && j.hasMoreElements()) {
            final String ref = (String) j.nextElement();
            if (props.getProperty(ref) == null) {
                res = false;
            }
        }
        return res;
    }

    /**
     *
     * @param props набор свойств
     * @param stack вложенный стек свойств
     * @param key Property name
     * @return признак установленной рекурсии
     */
    private static boolean exceedsDepth(final Properties props, final Stack<String> stack, final String key) {
        if (stack.size() > DEPTH) {
            return true;
        }
        stack.push(key);

        final Vector<String> fragments = new Vector<>();
        final Vector<String> refs = new Vector<>();
        final String val = props.getProperty(key);
        boolean res = false;

        parsePropertyString(val, fragments, refs);
        final Enumeration j = refs.elements();
        while (!res && j.hasMoreElements()) {
            final String ref = (String) j.nextElement();
            res = exceedsDepth(props, stack, ref);
            stack.pop();
        }
        return res;
    }

    /**
     *
     * @param props набор свойств
     * @param stack стек рекурсивного вызова
     * @param key Property name
     * @return признак установленной рекурсии
     */
    private static boolean hasRecursion(final Properties props, final Stack<String> stack, final String key) {

        stack.push(key);
        final Vector<String> fragments = new Vector<>();
        final Vector<String> refs = new Vector<>();
        final String val = props.getProperty(key);
        boolean res = false;

        parsePropertyString(val, fragments, refs);
        final Enumeration j = refs.elements();
        while (!res && j.hasMoreElements()) {
            final String ref = (String) j.nextElement();
            if (stack.contains(ref)) {
                res = true;
            } else {
                res = hasRecursion(props, stack, ref);
                stack.pop();
            }
        }
        return res;
    }

    /**
     * Читаем содержимое файла.
     * @param name Имя файла
     * @return содержимое файла как массив байт
     * @throws java.io.IOException IOException
     */
    public static byte[] loadFile(final String name) throws IOException {

        final ClassLoader ldr = ResourceLoader.class.getClassLoader();
        final InputStream in = ldr.getResourceAsStream(name);
        if (in != null) {
            try {
                final int sizefile = in.available();
                final byte[] imageData = new byte[sizefile];
                final int sizeLoaded = in.read(imageData);
                if (sizeLoaded != sizefile) {
                    throw new IOException(
                        "Число прочитанных байт(" + sizeLoaded
                        + ") не соответствует размеру файла \"" + name
                        + "\"(" + sizeLoaded
                        + ")!"
                    );
                }
                return imageData;
            } finally {
                in.close();
            }
        } else {
            throw new IOException("Файл \"" + name + "\" не найден!");
        }
    }

}
