<script setup lang="ts">
import type {Data as DataType} from "../type/Types.ts";
import type {PropType} from "vue";
defineProps({
  Data:{
    type:Object as PropType<DataType>,
    required:true
  }
})

</script>

<template>
  <div id="ShowBox">

    <div id="title">
      <div>{{Data.Tag}}</div><div>{{Data.Data[0].Value}}</div>
    </div>
    <div id="Date">
      <div>日期:</div><div>{{Data.Data[0].Date.getFullYear()+"-"+(Data.Data[0].Date.getMonth()+1)+"-"+Data.Data[0].Date.getDate()}}</div>
    </div>
  </div>
</template>

<style scoped>
#ShowBox{
  width: 200px;
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
</style>