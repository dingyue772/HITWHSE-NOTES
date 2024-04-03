import Vue from 'vue'
import VueRouter from 'vue-router'
Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    component: () => import(/* webpackChunkName: "about" */ '../views/userManagement/LoginView.vue')
  },
  {
    path: '/about',
    name: 'about',
    // route level code-splitting
    // this generates a separate chunk (about.[hash].js) for this route
    // which is lazy-loaded when the route is visited.
    component: () => import(/* webpackChunkName: "about" */ '../views/AboutView.vue')
  },
  // {
  //   path: '/pic-view',
  //   name: 'PicView',
  //   component: () => import(/* webpackChunkName: "about" */ '../views/infoManagement/PicView.vue')
  // },
  // {
  //   path: '/info-view',
  //   name: 'InfoView',
  //   component: () => import(/* webpackChunkName: "about" */ '../views/infoManagement/InfoView.vue')
  // },
  {
    path: '/Home',
    name: 'Home',
    component: () => import(/* webpackChunkName: "about" */ '../views/HomeView.vue'),
    children:[
      {
        path:"/info-view",
        name:"info-view",
        component: () => import(/* webpackChunkName: "about" */ '../views/infoManagement/InfoView.vue')
      },
      {
        path: '/pic-view',
        name: 'PicView',
        component: () => import(/* webpackChunkName: "about" */ '../views/infoManagement/PicView.vue')
      }
    ]
  }
]

const router = new VueRouter({
  routes
})

export default router
