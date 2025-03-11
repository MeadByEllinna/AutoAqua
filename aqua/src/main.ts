import { createApp } from 'vue'
import './style.css'
import App from './App.vue'
import ElementPlus from "element-plus";
import 'element-plus/dist/index.css'
import ReadMe from "./components/ReadMe.vue";
import Index from "./components/Index.vue";
import {createMemoryHistory, createRouter} from "vue-router";

const routes = [
    {path:'/',component:ReadMe},
    {path: '/index',component:Index }
]
const router = createRouter({
    history:createMemoryHistory(),
    routes
})

const app = createApp(App);
app.use(ElementPlus);
app.use(router);
app.mount('#app');

