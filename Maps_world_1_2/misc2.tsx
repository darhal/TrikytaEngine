<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="2018.11.29" name="misc2" tilewidth="32" tileheight="32" tilecount="6" columns="2">
 <image source="misc2.png" width="64" height="96"/>
 <tile id="1">
  <objectgroup draworder="index">
   <object id="1" x="4.90909" y="16" width="21.8182" height="16.1818">
    <properties>
     <property name="sensor" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="2">
  <objectgroup draworder="index">
   <object id="1" x="4.72727" y="15.8182" width="22.3636" height="16.5455">
    <properties>
     <property name="sensor" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="3">
  <objectgroup draworder="index">
   <object id="1" name="coin_detector" type="circle" x="8.66667" y="6.66667" width="15.6667" height="17.3333">
    <properties>
     <property name="sensor" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
  <animation>
   <frame tileid="3" duration="400"/>
   <frame tileid="4" duration="400"/>
  </animation>
 </tile>
 <tile id="5">
  <properties>
   <property name="sensor" type="bool" value="true"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" name="coeur" type="circle" x="3.33333" y="3.75758" width="27.8182" height="22.6364">
    <properties>
     <property name="sensor" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
  <animation>
   <frame tileid="5" duration="400"/>
   <frame tileid="0" duration="400"/>
  </animation>
 </tile>
</tileset>
