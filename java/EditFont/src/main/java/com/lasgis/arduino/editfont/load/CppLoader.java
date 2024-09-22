/*
 *  @(#)CppLoader.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.data.FontDataAdapter;
import com.lasgis.arduino.editfont.data.FontDataPerformed;
import com.lasgis.arduino.editfont.load.compile.ParseException;
import com.lasgis.arduino.editfont.load.compile.TokenParser;
import com.lasgis.util.Util;
import lombok.extern.slf4j.Slf4j;

import java.io.File;

/**
 * Загрузчик из *.data файла. Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
@Slf4j
public class CppLoader extends TokenParser {
    static final CppLoader CPP_LOADER = new CppLoader();

    public CppLoader() {
        super();
        FontDataPerformed.addListener(new FontDataAdapter() {
            @Override
            public void onChangeCFile(final File file) {
                CPP_LOADER.loadFile(file);
            }

            @Override
            public void onChangeCSource(final StringBuilder stringBuilder) {
                super.onChangeCSource(stringBuilder);
                try {
                    CPP_LOADER.parse(stringBuilder);
                } catch (final ParseException ex) {
                    log.error(ex.getMessage(), ex);
                }
            }
        });
    }

    private void loadFile(final File file) {
        final StringBuilder sb = Util.loadStringFromFile(file);
        FontDataPerformed.setCSource(sb);
    }

    public void parse(final StringBuilder stringBuilder) throws ParseException {
        setProgramCode(stringBuilder);
    }
/*
    public void parse(final FontData fontData) throws ParseException {
        final int beg = 0;
        final int end = sb.length() - 1;
        int i = beg;
        Token token;
        List<RomData> romDataList = new ArrayList<>();
        final List<BatchMemory> batchMemoryList = new ArrayList<>();
        final MemoryRoms memoryRoms = MemoryRoms.of("rom_memory", batchMemoryList);
        BatchMemory batchMemory = BatchMemory.of();
        batchMemory.setPrefix("batch");
        batchMemory.setDevice((byte) 0x00);
        batchMemory.setAddress(0x000);
        batchMemory.setRomDataList(romDataList);
        batchMemoryList.add(batchMemory);
        do {
            final TokenWrapper<MemoryRoms> memoryWrap = getMemoryRoms(i, end);
            if (nonNull(memoryWrap)) {
                final MemoryRoms memoryRomsWrap = memoryWrap.getData();
                memoryRoms.setHeaderFilename(memoryRomsWrap.getHeaderFilename());
                token = memoryWrap.getToken();
                i = token.beg;
            }
            final TokenWrapper<BatchMemory> batchWrap = getBatchMemory(i, end);
            if (nonNull(batchWrap)) {
                final BatchMemory bm = batchMemoryList.get(batchMemoryList.size() - 1);
                if (bm.getRomDataList().isEmpty()) {
                    batchMemoryList.remove(bm);
                }
                batchMemory = batchWrap.getData();
                romDataList = batchMemory.getRomDataList();
                batchMemoryList.add(batchMemory);
                token = batchWrap.getToken();
                i = token.beg;
            }
            final TokenWrapper<RomData> wrap = getRomData(i, end, null);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.beg;
            if (!(data instanceof RomEMPTY)) {
                romDataList.add(data);
            }
        } while (!token.is(TokenType.end));
        return memoryRoms;
    }
*/
}
