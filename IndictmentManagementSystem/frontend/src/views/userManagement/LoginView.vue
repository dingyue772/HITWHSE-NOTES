<template>
    <div class="all">
      <div class="login_box">
        <h1 class = 'logincss'>用户登录</h1>
          <el-form ref=loginForm :model="loginForm" :rules="loginFormRules" label-width="0" class="login_form" @keyup.enter.native='login'>
            <div>用户名</div>
            <el-form-item>
              <el-input v-model="loginForm.userName" prefix-icon="el-icon-user" placeholder="请输入用户名"></el-input>
            </el-form-item>
            <div>密码</div>
            <el-form-item>
              <el-input v-model="loginForm.password" prefix-icon="el-icon-lock" type="password" placeholder="请输入密码"></el-input>
            </el-form-item>
            <el-form-item size="large">
              <el-button type="primary" @click="login">登录</el-button>
              <el-button type="info" @click="registerDialogVisible=true">注册</el-button>
            </el-form-item>
          </el-form>
      </div>

      <!--定义注册对话框-->
        <el-dialog
          title="注册事件"
          :visible.sync="registerDialogVisible"
          width="30%">
          <el-form :model="registerForm" ref=registerForm :rules = "resgisterFormRules" @keyup.enter.native='register'>
            <el-form-item label="用户名称" prop="userName">
              <el-input v-model="registerForm.userName" placeholder="请输入用户名"></el-input>
            </el-form-item>
            <el-form-item label="密码" prop="password">
              <el-input v-model="registerForm.password" type="password" placeholder="请输入密码"></el-input>
            </el-form-item>
            <el-form-item label="确认密码" prop="password1">
              <el-input v-model="registerForm.password1" type="password" placeholder="请再次输入密码"></el-input>
            </el-form-item>
          </el-form>
          <span slot="footer" class="dialog-footer">
            <el-button @click="registerDialogVisible = false">取 消</el-button>
            <el-button type="primary" plain @click="register" @keyup.enter="register">确 定</el-button>
          </span>
        </el-dialog>
    </div>
</template>

<script>
// import {router} from '././router'
import axios from 'axios';
import {mapState} from 'vuex'

export default {
    data() {
      return {
        loginForm: {
          userName: '',
          password: ''
        },
        loginFormRules: {
        // 验证用户名是否合法
        userName: [
          { required: true, message: '请输入登录名称', trigger: 'blur' } // blur 触发方式 失去焦点
        ],
        // 验证密码是否合法
        password: [
          { required: true, message: '请输入登录密码', trigger: 'blur' },
          { min: 6, max: 15, message: '长度在 6 到 15 个字符', trigger: 'blur' }
        ]
      },
        registerDialogVisible:false,
        inputWidth:'50%',
        registerForm:{
          userName: '',
          password: '',
          repassword: ''
        },
        resgisterFormRules: {
        // 验证用户名是否合法
        userName: [
          { required: true, message: '请输入登录名称', trigger: 'blur' } // blur 触发方式 失去焦点
        ],
        // 验证密码是否合法
        password: [
          { required: true, message: '请输入注册密码', trigger: 'blur' },
          { min: 6, max: 15, message: '长度在 6 到 15 个字符', trigger: 'blur' }
        ],
        password1: [
          { required: true, message: '请再次输入确认密码', trigger: 'blur' },
          { min: 6, max: 15, message: '长度在 6 到 15 个字符', trigger: 'blur' }
        ]
      }
      };
    },
    computed: {
    ...mapState(['UserList'])
  },
    methods:{
         // login登录按钮
    login () {
      var me = this
      // 根据预验证是否发生请求
      this.$refs.loginForm.validate(valid => {
        if (!valid) return
        // 创建传给服务器的参数
        const params = {
          userName: this.loginForm.userName,
          password: this.loginForm.password
        }
        axios.post('/api/user/login', params)
          .then(function (response) {
            console.log(response.data['code'])
            if (response.data['code'] == 0) {
              console.log('登录成功')
              me.$store.state.IsLOGIn = true
              me.$router.push('/pic-view') // 实现页面跳转到Home.vue
              // me.$store.commit('editCurrentCase', me.loginForm.username)
              sessionStorage.setItem('currentUser', me.loginForm.userName)
              me.$message({
                message: '登录成功',
                type: 'success'
              })
            } 
            else if (response.data['code'] === 9999) {
              me.$message({
                message: '用户名或密码不正确',
                type: 'warning'
              })
              me.loginForm.password = ''
            } 
          })
          .catch(function (error) {
            me.$message({
              message: '错误' + error,
              type: 'error'
            })
            console.log('错误' + error)
          })
        this.loginForm.password = ''
      })
    },
    register () {
      const that = this
      var params={}
      if (this.registerForm.password !== this.registerForm.password1) {
        alert('两次输入的密码不一致')
        that.registerForm.password = ''
        that.registerForm.password1 = ''
      } else {
        params = {
          userName: this.registerForm.userName,
          password: this.registerForm.password,
          password1: this.registerForm.password1
        }
        axios.post('/api/user/register', params)
          .then(function (response) {
            if (response.data['code'] === 0) {
              console.log('注册成功')
              that.registerForm.userName = ''
              that.registerForm.password1 = ''
              that.registerForm.password = ''
              that.registerDialogVisible = false
              that.$message({
                message: response.data['msg'],
                type: 'success'
              })
            }
            else {
              that.$message({
                message: response.data['msg'],
                type: 'fail'
              })
              that.registerForm.userName = ''
              that.registerForm.password1 = ''
              that.registerForm.password = ''
            }
          })
          .catch(function (error) {
            console.log('错误' + error)
            that.$message.error('注册失败')
          })
      }
    },
  }
}
</script>

<style lang="less" scoped>
// 设置背景图片
.all{
  // height: 100%;
  // background-image: url("C:\Users\a\Desktop\JavaWeb\vue\vue-project\src\assets\background.jpg");
  
  background-repeat:no-repeat;
  background-size: cover;
}
.login_box {
  width: 25%;
  height: 40%;
  background-color: rgba(200,200,200,0.8);
  border-radius: 15px;
  position: absolute;
  right: 37.5%;
  top: 23%;
  // less语法嵌套
  .avatar_box {
    height: 130px;
    width: 130px;
    border: 1px solid #c4c5c7;
    border-radius: 50%;
    padding: 10px;
    box-shadow: 0 0 10px #ddd;
    position: absolute;
    left: 50%;
    transform: translate(-50%, -76%);
    background-color: #c4c5c7;
    img {
      width: 100%;
      height: 100%;
      border-radius: 50%;
      background-color: #c4c5c7;
    }
    .el-input {
      background-color: #c4c5c7;
    }
  }
}

.login_form {
  position: absolute;
  width: 100%;
  bottom: 0;
  padding: 0 20px;
  box-sizing: border-box;

}

.logincss{
  text-align:center;
  font-family: Helvetica, '_GB2312 FangSong_GB2312', Arial, sans-serif;
}
</style>