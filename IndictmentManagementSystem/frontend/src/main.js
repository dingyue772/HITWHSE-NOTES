import Vue from 'vue'
import App from './App.vue'
import router from './router'
// 引入ElementUI操作
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
import Vuex from 'vuex'


Vue.config.productionTip = false
Vue.use(ElementUI).use(Vuex);
const store = new Vuex.Store({
  IsLOGIn: false, // 记录当前的状态
  currentUser: '', // 记录当前登录的用户名信息
  mutations:{
    // 退出登录后IsLogin变为false
    Logout (state) {
      state.IsLOGIn = false
    },
  }
})
new Vue({
  store,
  router,
  render: h => h(App)
}).$mount('#app')
