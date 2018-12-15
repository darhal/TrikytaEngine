<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="2018.11.29" name="sheet_key" tilewidth="32" tileheight="32" tilecount="4" columns="4">
 <image source="sheet_key.png" width="128" height="41"/>
 <tile id="0" type="circle">
  <properties>
   <property name="sensor" type="bool" value="true"/>
  </properties>
  <objectgroup draworder="index">
   <object id="5" x="9.75" y="-5" height="0.875"/>
   <object id="9" x="4.36364" y="7.81818" width="25.2727" height="25.4545">
    <ellipse/>
   </object>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="150"/>
   <frame tileid="1" duration="150"/>
   <frame tileid="2" duration="150"/>
   <frame tileid="3" duration="150"/>
  </animation>
 </tile>
</tileset>
