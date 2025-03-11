
import ReadMe from "../components/ReadMe.vue";
import Index from "../components/Index.vue";
import  Setting from "../components/Setting.vue";
import  CheckOut from "../components/CheckOut.vue";
import {createMemoryHistory, createRouter} from "vue-router";

const routes = [
    {path:'/',component:ReadMe},
    {path: '/index',component:Index },
    {path:'/setting',component:Setting },
    {path:'/checkout',component:CheckOut }
]

const router = createRouter({
    history:createMemoryHistory(),
    routes
})
export default router;