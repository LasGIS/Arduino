/*
 *  @(#)DataXmlLoader.java  last: 31.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.CommonInfoException;
import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomDOUBLE;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomFLOAT;
import com.lasgis.arduino.eeprom.memory.RomINT16;
import com.lasgis.arduino.eeprom.memory.RomINT32;
import com.lasgis.arduino.eeprom.memory.RomINT8;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import com.lasgis.arduino.eeprom.memory.RomSTRING;
import lombok.extern.slf4j.Slf4j;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Загрузчик из XML файла.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
@Slf4j
class DataXmlLoader {

    private final static String UNKNOWN_FORMAT = "Формат XML файла: ";
    private final static String ROM = "ROM";
    private final static String CHAR = "CHAR";
    private final static String INT8 = "INT8";
    private final static String INT16 = "INT16";
    private final static String INT32 = "INT32";
    private final static String FLOAT = "FLOAT";
    private final static String DOUBLE = "DOUBLE";
    private final static String STRING = "STRING";
    private final static String OBJECT = "OBJECT";
    private final static String ARRAY = "ARRAY";

    private DocumentBuilder dBuilder;
    private List<RomData> list = new ArrayList<>();

    private DataXmlLoader() throws ParserConfigurationException {
        dBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
    }

    static List<RomData> load(final File file) throws Exception {
        final DataXmlLoader loader = new DataXmlLoader();
        loader.loadFile(file);
        return loader.list;
    }

    private void loadFile(final File file) throws Exception {
        list.clear();

        final Document doc = dBuilder.parse(file);
        final Element first = doc.getDocumentElement();
        if (!ROM.equals(first.getNodeName())) {
            throw new CommonInfoException(
                UNKNOWN_FORMAT + "первый элемент = \"{0}\", а должен быть \"ROM\"",
                first.getNodeName()
            );
        }
        final NodeList nodeList = first.getChildNodes();

        for (int i = 0; i < nodeList.getLength(); i++) {
            final Node node = nodeList.item(i);
            final RomData data = nodeToData(node);
            if (data != null) {
                list.add(data);
            }
        }
    }

    private RomData nodeToData(final Node node) {
        final NamedNodeMap attrs = node.getAttributes();
        final String name;
        final String value;
        if (attrs != null) {
            final Node nodeName = attrs.getNamedItem("name");
            name = (nodeName != null) ? nodeName.getNodeValue() : null;
            final Node nodeValue = attrs.getNamedItem("val");
/*
            if (nodeValue == null) {
                throw new CommonInfoException(
                    UNKNOWN_FORMAT + "Пропущен атрибут \"value\" для \"{0}\"!", node.getNodeName()
                );
            }
*/
            value = (nodeValue != null) ? nodeValue.getNodeValue() : null;
        } else {
            name = null;
            value = null;
        }
        switch (node.getNodeName()) {
            case CHAR:
                return RomCHAR.of(name, value.charAt(0));
            case INT8:
                return RomINT8.of(name, Byte.parseByte(value));
            case INT16:
                return RomINT16.of(name, Short.parseShort(value));
            case INT32:
                return RomINT32.of(name, Integer.parseInt(value));
            case FLOAT:
                return RomFLOAT.of(name, Float.parseFloat(value));
            case DOUBLE:
                return RomDOUBLE.of(name, Double.parseDouble(value));
            case STRING:
                return RomSTRING.of(name, value);
            case OBJECT:
                return addSubNode(RomOBJECT.of(name), node);
            case ARRAY:
                return addSubNode(RomARRAY.of(name), node);
            case "#comment":
            case "#text":
                return null;
            default:
                throw new CommonInfoException(
                    UNKNOWN_FORMAT + "Неизвестный элемент = \"{0}\"!", node.getNodeName()
                );
        }
    }

    private RomData addSubNode(final RomData rom, final Node node) {
        final NodeList nodeList = node.getChildNodes();
        for (int i = 0; i < nodeList.getLength(); i++) {
            final Node child = nodeList.item(i);
            final RomData data = nodeToData(child);
            if (data != null) {
                if (rom instanceof RomARRAY) {
                    ((RomARRAY) rom).add(data);
                } else if (rom instanceof RomOBJECT) {
                    ((RomOBJECT) rom).add(data);
                }
            }
        }
        return rom;
    }
}
