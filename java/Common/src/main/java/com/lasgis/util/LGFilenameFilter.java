/*
 * @(#)LGFilenameFilter.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2013 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.io.FilenameFilter;
import java.io.File;

/**
 * Created by IntelliJ IDEA.
 * @author vlaskin
 * @version 1.0
 * @since 14.09.2005 17:23:43
 */
public class LGFilenameFilter implements FilenameFilter {

    /** расширение файла. */
    private String ext = null;

    /**
     * Constructor.
     * @param aMask маска файла
     */
    public LGFilenameFilter(String aMask) {
        int lastPnt = aMask.lastIndexOf('.');
        if (lastPnt > 0) {
            ext = aMask.substring(lastPnt);
        }
    } // LGFilenameFilter(String aMask)

    /**
     * Tests if a specified file should be included in a file list.
     *
     * @param   dir    the directory in which the file was found.
     * @param   name   the name of the file.
     * @return  <code>true</code> if and only if the name should be
     * included in the file list; <code>false</code> otherwise.
     */
    public boolean accept(File dir, String name) {
        if (dir.isDirectory()) {
            return false;
        }
        if (ext != null) {
            String fileName = dir.getName();
            String relExt = null;
            int lastPnt = fileName.lastIndexOf('.');
            if (lastPnt > 0) {
                relExt = fileName.substring(lastPnt);
            }
            if ((relExt != null) && (relExt.equalsIgnoreCase(ext))) {
                return true;
            }
        }
        return false;
    }
}
