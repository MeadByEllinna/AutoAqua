<script setup lang="ts">
import StatisticsResults from "./components/StatisticsResults.vue";
import type {SingleData,Data} from "./type/Types.ts";
const TestSingleData: SingleData = {
  Value:8.5,
  Date:new Date()
}
const DataLst:Array<SingleData> = [TestSingleData]
const TestData: Data = {
  Tag:"KH",
  Data:DataLst
}
</script>

<template>
  <div id="Layout">
    <div id="top">
      <div id="Header">
        <img src="./assets/img/coral.png" alt="logo" />
        <div id="title"> Auto Aqua 自动水质监测</div>
      </div>
    </div>
    <div id="middle">
      <div id="Content">
        <div id="ReefIndexShow">
          <StatisticsResults :Data="TestData"></StatisticsResults>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
#Layout {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
}
#top{
  width: 100%;
  height: 100px;
}
#Header{
  width: 100%;
  height: 100%;
  display: flex;
}
#Header>img{
  height: 90px;
  margin-top: 5px;
  margin-left: 30px;
}
#title{
  line-height: 100px;
}
#ReefIndexShow{
  width: 50%;
  height: 50%;
}
#middle{
  display: flex;
  flex-grow: 1;
}
#Content{
  flex-grow: 1;;
}
</style>
