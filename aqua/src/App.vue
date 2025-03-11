<script setup lang="ts">

import {ref} from 'vue';
import {useRouter} from "vue-router";

const activeIndex = ref<String>('0');
const router = useRouter();
const handleSelect = (key: string) => {
  activeIndex.value = key;
  switch (key){
    case '0':
      router.push({path:'/'});
      break;
    case '1':
      router.push({path:'/index'});
      break;
    case '2':
      router.push({path:'/checkout'});
      break;
    case '3':
      router.push({path:'/setting'});
      break;
    default:
      break;
  }
}
</script>

<template>
  <div id="Layout">
    <div id="top">
      <div id="NavMenu">
        <el-menu
            :default-active="activeIndex"
            class="el-menu-demo"
            mode="horizontal"
            :ellipsis="false"
            @select="handleSelect"
        >
          <el-menu-item index="0">
            <img
                style="height:100%"
                src="./assets/img/coral.png"
                alt="Element logo"
            />
          </el-menu-item>
          <el-menu-item index="1">首页</el-menu-item>
          <el-menu-item index="2">检测</el-menu-item>
          <el-menu-item index="3">设置</el-menu-item>
        </el-menu>
      </div>
    </div>
    <div id="middle">
      <div id="Content">
        <router-view></router-view>
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
#NavMenu{
  height: 20%;
  width: 100%;
  display: flex;
  padding-top: 20px;
  box-sizing: border-box;
}
#Header{
  width: 100%;
  height: 75%;
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
}
#middle{
  display: flex;
  flex-grow: 1;
}
#Content{
  flex-grow: 1;
}
.el-menu{
  width: 100%;
}
</style>
