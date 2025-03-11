<script setup lang="ts">
import type {Data as DataType} from "../type/Types.ts";
import { type PropType, onMounted} from "vue";
import  {ref} from "vue";
import * as echarts from "echarts";
const props = defineProps({
  Data:{
    type:Object as PropType<DataType>,
    required:true
  }
})
const charts =ref();
onMounted(()=>{
  const dateTag= props.Data.Data.map(item=>item.Date.toLocaleDateString());
  const valueTag =props.Data.Data.map(item=>item.Value);
  var myCharts=echarts.init(charts.value);
  myCharts.setOption({
    tooltip: {},
    xAxis: {
      data: dateTag
    },
    yAxis: {
      type:'value',
      interval:1,
      min:5,
      max:10
    },
    series: [
      {
        name: 'Kh(dkh)',
        type: 'line',
        smooth:true,
        data: valueTag
      }
    ]
  });
  myCharts.on("click",params=>{
    console.log(params);
  })
})

</script>

<template>
  <div id="ShowBox">

    <div id="DataBox">
      <div id="title">
        <div>{{Data.Tag}}</div><div>{{Data.Data[0].Value}}</div>
      </div>
      <div id="Date">
        <div>日期:</div><div>{{Data.Data[0].Date.getFullYear()+"-"+(Data.Data[0].Date.getMonth()+1)+"-"+Data.Data[0].Date.getDate()}}</div>
      </div>
    </div>
    <div id="ChartsBox">
      <div id="charts" ref="charts"></div>
    </div>
  </div>
</template>

<style scoped>
#ShowBox{
  width: 900px;
  display: flex;
  cursor: pointer;
}
#ShowBox:hover{
  box-shadow: aqua 5px 10px 10px;
}
#DataBox{
  width: 200px;
  height: 200px;
  align-content: center;
}
#title{
  display: flex;
}
#title>div{
  width: 50%;
  flex: 1;
  text-align: center;
  font-weight: bold;
  font-size: xx-large;
}
#Date{
  display: flex;
}
#Date>div{
  text-align: center;
  width: 50%;
  font-weight: bold;
}
#charts{
  width: 700px;
  height: 200px;
}
</style>