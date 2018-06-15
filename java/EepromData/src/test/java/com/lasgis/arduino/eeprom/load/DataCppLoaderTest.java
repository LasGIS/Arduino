package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomDOUBLE;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomINT16;
import com.lasgis.arduino.eeprom.memory.RomINT32;
import com.lasgis.arduino.eeprom.memory.RomINT8;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import com.lasgis.arduino.eeprom.memory.RomSTRING;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import java.util.ArrayList;

import static org.testng.Assert.*;

/**
 * @author Vladimir Laskin
 * @since 15.06.2018
 */
public class DataCppLoaderTest {

    @DataProvider
    public Object[][] dataLoad() {
        return new Object[][]{{
            "  /* Комментарий */\n" +
                "  CHAR CHAR_SINGN_WIN : 'B';\n" +
                "  INT8 : 255;\n" +
                "  INT16 : 65535;\n" +
                "  INT16 : -32767;\n" +
                "  INT32 : 255;\n" +
                "  DOUBLE : 3.1415926;\n" +
                "  STRING WIN_NAME : \"Название машинки\";\n" +
                "  OBJECT_VEHICLE : {\n" +
                "     INT8 : 255,\n" +
                "     DOUBLE : 60.78,\n" +
                "     STRING : \"Быстрый мустанг\"\n" +
                "  };\n",
            new ArrayList<RomData>() {{
                this.add(RomCHAR.of("CHAR_SINGN_WIN", 'B'));
                this.add(RomINT8.of(255));
                this.add(RomINT16.of(65535));
                this.add(RomINT16.of(-32767));
                this.add(RomINT32.of(255));
                this.add(RomDOUBLE.of(3.1415926));
                this.add(RomSTRING.of("WIN_NAME", "Название машинки"));
                this.add(RomOBJECT.of("OBJECT_VEHICLE")
                    .add(RomINT8.of(255))
                    .add(RomDOUBLE.of(60.78))
                    .add(RomSTRING.of("Быстрый мустанг"))
                );
            }}
        }};
    }

    @Test(dataProvider = "dataLoad")
    public void testLoad(
        final String data,
        final ArrayList<RomData> expectedList
    ) throws Exception {
        DataCppLoader loader = new DataCppLoader(new StringBuilder(data));
        loader.parse();
    }

}