<template>
  <div class="bigDiv">
    <el-container class="home-container" style="height: 1000px; border: 1px solid #eee">
      <!--头部区域-->
      <el-header>
        <div>
          <span>起诉书信息提取和展示工具</span>
        </div>
        <el-dropdown>
        <span class="el-dropdown-link">
          {{userName}}<i class="el-icon-arrow-down el-icon--right"></i>
        </span>
          <el-dropdown-menu slot="dropdown">
            <el-dropdown-item @click.native="logout" class="el-icon-circle-close"> 退出</el-dropdown-item>
          </el-dropdown-menu>
        </el-dropdown>
      </el-header>
      <!--页面主体区域-->
      <el-container class="myContainer">
        <!--侧边栏-->
        <!--isCollapse 判断是否折叠-->
        <el-aside :width="isCollapse ? '64px' : '200px'">
          <!--菜单栏-->
          <el-menu
            background-color="#313744"
            text-color="#fff"
            default-active="2"
            class="el-menu-vertical-demo"
            active-text-color="#0F96FF"
            :router="true">
            <el-menu-item index="pic-view">
              <i class="el-icon-document"></i>
              <span slot="title">起诉书管理模块</span>
            </el-menu-item>
            <el-menu-item index="info-view">
              <i class="el-icon-setting"></i>
              <span slot="title">识别结果管理模块</span>
            </el-menu-item>
          </el-menu>
        </el-aside>
        <el-main>
          <!--路由占位符-->
          <router-view></router-view>
        </el-main>
      </el-container>
    </el-container>

  </div>

</template>

<script>
export default {
  data () {
    return {
      userName: sessionStorage.getItem('currentUser'),
      // 是否折叠
      isCollapse: false,
    }
  },
  methods: {
    // 退出登录按钮
    logout () {
      window.sessionStorage.clear()
      this.$store.commit('Logout')
      this.$router.push('/')
      // Vue.prototype.$message('已退出登录');
    }
  }
}
</script>

<style lang="less" scoped>
.bigDiv {
  height: 100%;
}
.home-container {
  height: 100%;
}
.el-header {
  background-color: #373D41;
  display: flex;
  justify-content: space-between;
  padding-left: 0;
  align-items: center;
  color: #ffffff;
  font-size: 20px;
  > div {
    display: flex;
    align-items: center;
    span {
      margin-left: 15px;
    }
  }
}

.el-dropdown {
  margin-right: 15px;
  span {
    color: #ffffff;
  }
}
.menu-container{
  height: 100%;
}

.el-aside {
  background-color: #333744;
  //解决右侧边框线对不齐的问题
  .el-menu {
    border-right: none;
  }
}

.el-main {
  background-color: #E8EDF0;
  padding: 0;
}

.toggle-button {
  background-color: #4A5064;
  color: #ffffff;
  font-size: 10px;
  height: 24px;
  text-align: center;
  line-height: 24px;
  //字间距
  letter-spacing: 0.2em;
  //鼠标变成小手
  cursor: pointer;
}
.el-dropdown-menu{
  width: 120px;
}
.el-dropdown-link {
  cursor: pointer;
}
.myContainer {
  height: 100%;
}
</style>
