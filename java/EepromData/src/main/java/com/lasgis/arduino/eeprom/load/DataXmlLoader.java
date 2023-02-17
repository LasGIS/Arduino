/*
 *  @(#)DataXmlLoader.java  last: 17.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.CommonInfoException;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
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

import javax.xml.bind.DatatypeConverter;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import static com.lasgis.util.Util.parseCharAt0;
import static com.lasgis.util.Util.parseDoubleOfNull;
import static com.lasgis.util.Util.parseFloatOfNull;
import static com.lasgis.util.Util.parseHexByte;
import static com.lasgis.util.Util.parseHexInt;
import static com.lasgis.util.Util.parseHexShort;

/**
 * Загрузчик из XML файла.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
@Slf4j
class DataXmlLoader {

    private final static String UNKNOWN_FORMAT = "Формат XML файла: ";
    private final static String MEMORY = "Memory";
    private final static String BATCH_MEMORY = "BatchMemory";
    private final static String CHAR = "CHAR";
    private final static String INT8 = "INT8";
    private final static String INT16 = "INT16";
    private final static String INT32 = "INT32";
    private final static String FLOAT = "FLOAT";
    private final static String DOUBLE = "DOUBLE";
    private final static String STRING = "STRING";
    private final static String OBJECT = "OBJECT";
    private final static String ARRAY = "ARRAY";
    private final static String DUMP = "DUMP";

    private final DocumentBuilder dBuilder;

    private DataXmlLoader() throws ParserConfigurationException {
        dBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
    }

    static MemoryRoms load(final File file) throws Exception {
        final DataXmlLoader loader = new DataXmlLoader();
        return loader.loadFile(file);
    }

    private MemoryRoms loadFile(final File file) throws Exception {
        final List<BatchMemory> list = new ArrayList<>();
        final Document doc = dBuilder.parse(file);
        final Element first = doc.getDocumentElement();
        if (!MEMORY.equals(first.getNodeName())) {
            throw new CommonInfoException(
                UNKNOWN_FORMAT + "первый элемент = \"{0}\", а должен быть \"{1}\"",
                first.getNodeName(), MEMORY
            );
        }
        final NamedNodeMap firstAttributes = first.getAttributes();
        final Node nodeFilename = firstAttributes.getNamedItem("headerFilename");
        final String headerFilename = (nodeFilename != null) ? nodeFilename.getNodeValue() : "rom_memory";

        final NodeList memoryList = first.getChildNodes();
        for (int j = 0; j < memoryList.getLength(); j++) {
            final Node batchNode = memoryList.item(j);
            if (BATCH_MEMORY.equals(batchNode.getNodeName())) {
                final BatchMemory batchMemory = BatchMemory.of();
                final NamedNodeMap attrs = batchNode.getAttributes();

                final Node nodeDevice = attrs.getNamedItem("device");
                final byte device = (nodeDevice != null) ? parseHexByte(nodeDevice.getNodeValue()) : 0x00;
                batchMemory.setDevice(device);

                final Node nodeAddress = attrs.getNamedItem("address");
                final int address = (nodeAddress != null) ? parseHexInt(nodeAddress.getNodeValue()) : 0;
                batchMemory.setAddress(address);

                final Node nodePrefix = attrs.getNamedItem("prefix");
                final String prefix = (nodePrefix != null) ? nodePrefix.getNodeValue() : "";
                batchMemory.setPrefix(prefix);

                final NodeList nodeList = batchNode.getChildNodes();
                for (int i = 0; i < nodeList.getLength(); i++) {
                    final Node node = nodeList.item(i);
                    final RomData data = nodeToData(node);
                    if (data != null) {
                        batchMemory.getRomDataList().add(data);
                    }
                }
                list.add(batchMemory);
            }
        }
        return MemoryRoms.of(headerFilename, list);
    }

    private RomData nodeToData(final Node node) {
        final NamedNodeMap attrs = node.getAttributes();
        final String name;
        final String value;
        if (attrs != null) {
            final Node nodeName = attrs.getNamedItem("name");
            name = (nodeName != null) ? nodeName.getNodeValue() : null;
            final Node nodeValue = attrs.getNamedItem("val");
            value = (nodeValue != null) ? nodeValue.getNodeValue() : null;
        } else {
            name = null;
            value = null;
        }
        switch (node.getNodeName()) {
            case CHAR:
                return RomCHAR.of(name, parseCharAt0(value));
            case INT8:
                return RomINT8.of(name, parseHexByte(value));
            case INT16:
                return RomINT16.of(name, parseHexShort(value));
            case INT32:
                return RomINT32.of(name, parseHexInt(value));
            case FLOAT:
                return RomFLOAT.of(name, parseFloatOfNull(value));
            case DOUBLE:
                return RomDOUBLE.of(name, parseDoubleOfNull(value));
            case STRING:
                return RomSTRING.of(name, value);
            case OBJECT:
                return addSubNode(RomOBJECT.of(name), node);
            case ARRAY:
                return addSubNode(RomARRAY.of(name), node);
            case DUMP:
                return dumpNode(name, value, node);
            case "#comment":
            case "#text":
                return null;
            default:
                throw new CommonInfoException(
                    UNKNOWN_FORMAT + "Неизвестный элемент = \"{0}\"!", node.getNodeName()
                );
        }
    }

    private RomData dumpNode(final String name, final String value, final Node node) {
        final RomARRAY rom = RomARRAY.of(name);
        String dumpStr = Objects.nonNull(value) ? value : node.getTextContent().trim().replaceAll("[ \n\t\r]", "");
        byte[] dump = DatatypeConverter.parseHexBinary(dumpStr);
        for (final byte bt : dump) {
            rom.add(RomINT8.of(bt));
        }
        return rom;
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
