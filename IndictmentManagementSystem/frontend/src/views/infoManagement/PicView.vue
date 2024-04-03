<template>
    <div>
      <!--根据条件查询起诉书-->
      <el-form :inline="true" :model="searchForm" class="demo-form-inline" style="border: 1px solid #eee">
      <el-form-item label="起诉书名称">
          <el-input v-model="searchForm.indictment_name"></el-input>
      </el-form-item>
      <el-form-item label="操作人">
          <el-input v-model="searchForm.person_name"></el-input>
      </el-form-item>
      <el-form-item>
          <span class="demonstration">起止时间</span>
          <el-date-picker v-model="searchForm.date" type="daterange" range-separator="至" start-placeholder="开始日期" end-placeholder="结束日期"></el-date-picker>
      </el-form-item>
      <el-form-item>
          <el-button type="primary" @click="query_for_indictments">查询</el-button>
          <el-button type="primary" @click="handleReset">重置</el-button>
      </el-form-item>
      </el-form>

      <br/>
      <div style="display: flex; justify-content: space-between; align-items: center;">
      <!--新增起诉书按钮-->
      <el-button type="primary" @click="addInfoDialogVisible=true"  style="margin-left: 20px;">新增起诉书</el-button>
      </div>
      <!--起诉书图片信息展示表格-->
      <el-table :data="indictmentRows">
          <el-table-column prop="indictment_name" label="起诉书名称" width="200%"></el-table-column>
          <el-table-column prop="person_name" label="操作人" width="70%"></el-table-column>
          <el-table-column label="起诉书图片下载">
            <template slot-scope="scope">
              <div v-for="picName in scope.row.picNameList" :key="picName">
                <a :href="`api/indictment/getPic?indictment_name=${scope.row.indictment_name}&picName=${picName}`">{{picName}}</a>
              </div>
            </template>
          </el-table-column>
          <el-table-column prop="status" label="识别状态" width="85%"></el-table-column>
          <el-table-column label="操作" width="300%">
          <template v-slot="scope">
              <div style="display: flex; justify-content: space-between;">
                  <el-button size="mini" @click="changeInfoDialog(scope.row)">修改</el-button>
                  <el-button size="mini" @click="pic_to_text(scope.row)">图片识别</el-button>
                  <el-button size="mini" @click="picDelete(scope.row)">删除</el-button>
                  <el-button size="mini" @click="showDetails(scope.row)">查看详情</el-button>
              </div>
          </template>
          </el-table-column>
      </el-table>
      <!--表格的分页设计-->
      <div class="block">
        <!--layout中表示当前显示的是上一页选项，展示页码和下一页选项，page-sizes是可选的每页记录数，page-size是当前的每页记录数目-->
        <el-pagination
        layout="prev, pager, next"  
        :current-page="currentPage"
        :page-size="pageSize"
        :total="total"
        @current-change="handlePageChange"
      >
      </el-pagination>
      </div>  

      <!--新增起诉书弹框设置-->
      <el-dialog title="新增起诉书信息" :visible.sync="addInfoDialogVisible" width="30%">
          <el-form :model="newIndictment">
              <el-form-item label="起诉书名称" >
              <el-input v-model="newIndictment.indictment_name" autocomplete="off"></el-input>
              </el-form-item>
              <!-- action="api/indictment/add"  -->
              <el-form-item label="起诉书图片上传">
                  <el-upload
                  ref="upload"
                  multiple 
                  action="#"
                  :auto-upload="false"
                  :before-remove="beforeRemove"
                  accept="image/jpeg,image/gif,image/png"
                  :before-upload="onBeforeUpload"
                  :limit="100"
                  :file-list="newPicList"
                  list-type="picture"
                  :on-change="Upload_file"
                   >
                   
                    <el-button size="small" type="primary">点击上传</el-button>
                  </el-upload>
              </el-form-item>
          </el-form>
          <div slot="footer" class="dialog-footer">
              <el-button @click="addInfoDialogVisible=false">取 消</el-button>
              <el-button type="primary" @click="confirmUpload">确 定</el-button>
          </div>
      </el-dialog>

      <!--修改起诉书弹框管理-->
      <el-dialog title="新增起诉书信息" :visible.sync="changeInfoDialogVisible" width="30%">
          <el-form :model="changeIndictment">
              <el-form-item label="起诉书名称" >
              <el-input v-model="changeIndictment.indictment_name" autocomplete="off"></el-input>
              </el-form-item>
              <!-- action="api/indictment/add"  -->
              <el-form-item label="起诉书图片上传">
                  <el-upload
                  ref="change_upload"
                  multiple 
                  action="#"
                  :auto-upload="false"
                  :before-remove="beforeRemove"
                  accept="image/jpeg,image/gif,image/png"
                  :before-upload="onBeforeUpload"
                  :limit="100"
                  :file-list="changePicList"
                  list-type="picture"
                  :on-change="change_Upload_file"
                   >
                    <el-button size="small" type="primary">点击上传</el-button>
                  </el-upload>
              </el-form-item>
          </el-form>
          <div slot="footer" class="dialog-footer">
              <el-button @click="changeInfoDialogVisible=false">取 消</el-button>
              <el-button type="primary" @click="changePicSubmit">确 定</el-button>
          </div>
      </el-dialog>

      <!--查看详情的对话框-->
      <el-dialog title="起诉书详情展示" :visible.sync="showDetailsDialogVisible">
        <el-form :model="showIndictment">
              <el-form-item label="起诉书名称" >
              <div>{{showIndictment.indictment_name}}</div>
              </el-form-item>
              <el-form-item label="操作人名称" >
              <div>{{showIndictment.person_name}}</div>
              </el-form-item>
              <el-form-item label="创建时间" >
              <div>{{showIndictment.create_time}}</div>
              </el-form-item>
              <el-form-item label="更新时间" >
              <div>{{showIndictment.update_time}}</div>
              </el-form-item>
              <el-form-item label="识别状态" >
              <div>{{showIndictment.status}}</div>
              </el-form-item>
        </el-form>
        <el-row v-for="(pic, index) in showIndictment.picList" :key="index">
          <div class="pic-name-show">{{showIndictment.picNameList[index]}}</div>
          <div>
            <el-image class="showPics" :src="pic"></el-image>
          </div>
        </el-row>
      </el-dialog>

      <!--识别图片的对话框定义-->
      <el-dialog title="起诉书图片识别结果" :visible.sync="picInfoGetDialogVisible">
        <el-row v-for="(pic, index) in recognize_indictment_pics" :key="index">
          <el-col :span="17">
            <span class="mySpan,pic-name-show">图片</span>
            <div class="grid-content bg-purple my-span-div" >
              <div class="demo-image__placeholder">
                <div class="block">
                  <span class="demonstration"></span>
                  <div>
                    <el-image class="recognizePics" :src="pic"></el-image>
                  </div>
                  
                </div>
              </div>
            </div>
          </el-col>
          <el-col :span="7" class="myText">
            <span class="mySpan,pic-name-show">识别文本</span>
            <div class="grid-content bg-purple-light">
              <div>
                {{recognize_indictment_texts[index]}}
              </div>
            </div>
          </el-col>
        </el-row>
          <div slot="footer" class="dialog-footer">
                  <el-button @click="picInfoGetDialogVisible=false">取 消</el-button>
                  <el-button type="primary" @click="picInfoGetDialogVisible=false">确 定</el-button>
          </div>
      </el-dialog>
    </div>
</template>

<script>
import axios from 'axios';
export default {
    data() {
      return {
        // 新增起诉书时传递给后端的formData
        // newIndictmentFormData:'',
        // file:'',
        completeRows:[], // 查询得到的全部起诉书记录
        newPicList:[], // 存储新建起诉书时上传的图片
        changePicList:[], // 存储修改起诉书时上传的图片
        userName:sessionStorage.getItem('currentUser'),
        //与分页有关的数据
        currentPage:1,
        pageSize:5,
        total:0,
        // 表格中显示的起诉书信息模型
        // indictmentRows: [],
        // 搜索条件模型
        searchForm:{
          indictment_name:'',
          person_name:'',
          date:[]
        }, 
        // 新增的起诉书信息模型
        newIndictment:{
            indictment_name:'',
        },
        // 修改起诉书对话框中的信息模型
        changeIndictment:{
            indictment_name:'',
        },
        // 当前查看详情的起诉书信息模型
        showIndictment:{
          indictment_name:'',
        },
        // 控制新增起诉书弹框显示的变量
        addInfoDialogVisible:false,
        // 控制修改起诉书弹框显示的变量
        changeInfoDialogVisible:false,
        // 控制起诉书图片识别弹框显示的变量
        picInfoGetDialogVisible:false,
        // 定义确定删除起诉书的对话框显示变量
        confirm_deleteDialogVisble:false,
        showDetailsDialogVisible:false,
        recognize_indictment_pics:[],
        recognize_indictment_texts:[]
      }
    },
    computed:{
        indictmentRows(){
            const start = (this.currentPage-1)*this.pageSize;
            const end = start+this.pageSize;
            return this.completeRows.slice(start, end);
        }
    },
    methods:{
          Upload_file(file, fileList){       // 对应的是新增起诉书弹窗中的上传文件，一旦上传文件修改就将this.newPicList对应修改
            this.newPicList = fileList;
          },
          change_Upload_file(file, fileList){       // 对应的是修改起诉书弹窗中的上传文件，一旦上传文件修改就将this.newPicList对应修改
            this.changePicList = fileList;
          },
        // 新增起诉书成功事件
        confirmUpload(){
            this.newIndictmentFormData = new FormData();
            this.newIndictmentFormData.append('indictment_name', this.newIndictment.indictment_name);
            this.newIndictmentFormData.append('person_name', this.userName);
            this.newIndictmentFormData.append('status', 0);
            for (const file of this.newPicList) {
              console.log(file.raw)
              this.newIndictmentFormData.append('files', file.raw);
              }
              axios.post('api/indictment/add', this.newIndictmentFormData)
              .then(response=> {
                if (response.data['code']==0) {
                  this.$message({
                    message:"新增起诉书成功",
                    type:'success'
                  })
                  // this.$refs.upload.clearFiles()              
                }                  
                else if (response.data['code']==9999)
                  this.$message({
                    message:response.data['msg'],
                    type:'warning'
                  })
              })
              .catch(error =>{
                  alert("发生错误",error);
              });
              this.newIndictment.indictment_name='';
              this.newPicList=[];
              this.addInfoDialogVisible=false;
        },
        // 修改起诉书成功事件
        changePicSubmit(){
            // const isModified = 1;
            // 检查起诉书信息是否发生变化,待修改——还需要检查picList中图片是否发生变化(通过upload的ElementUI是否有对应的操作来判断？)
            // const isModified = this.changeIndictment.indictment_name !== this.changeIndictmentRow.indictment_name;
            // if (!isModified) {
            //   this.changeInfoDialogVisible = false; // 关闭修改起诉书信息对话框
            //   return;
            // }
            const formData = new FormData();
            formData.append('indictment_name', this.changeIndictment.indictment_name);
            for (const file of this.changePicList) {
              console.log(file.raw)
              formData.append('files', file.raw);
            }
            formData.append('person_name', this.userName);
            formData.append('status', this.changeIndictment.status);
            axios.post('/api/indictment/update', formData)
            .then(response => {              
              if (response.data['code']==0)
              this.$message({
                    message:"修改起诉书成功",
                    type:'success'
                  })
              else if (response.data['code']==9999)
                console.log("部分传输数据为空");
              // 处理响应结果
              this.changeIndictment={};
              this.changeIndictment.indictment_name='';
              this.changeInfoDialogVisible = false; // 关闭修改起诉书信息对话框
              // 其他操作...
            })
            .catch(error => {
              // 处理错误
              alert("发生错误",error);
            });
        },
        // 删除起诉书事件
        picDelete(indictment){
            const ind = {...indictment}
            const params = {
              indictment_name:ind['indictment_name']
            }
            axios.post("/api/indictment/delete", params).
        then(response =>{
          if (response.data['code'] == 0)
          this.$message({
                    message:"删除起诉书成功",
                    type:'success'
                  })
        })
        .catch(error=>{
          console.log(error);
        }
        )
        },
        // 查询符合条件的indictments
       async query_for_indictments(){
        try{
          // 获取查询条件
          const { indictment_name, person_name, date } = this.searchForm;
          const [start_time, end_time] = date;
          console.log(date);
          console.log(start_time);
          console.log(end_time);
          // 构造请求参数
          const params = {
            indictment_name,
            person_name,
            start_time,
            end_time,
            page:this.currentPage, // 当前页
            per_page:this.pageSize // 每页的记录数
          }
          console.log(params['start_time']);
          console.log(params['end_time']);
          // 发送 POST 请求到后端
          const response = await axios.post('/api/indictment/query', params);
          if (response.data['code']==0){
              // 处理响应结果
            // console.log('11111',response.data['data']);
          // 更新页面数据或执行其他操作
          this.total=response.data['data']['recordCount'];
          //console.log(response.data['data']['rows'][0])
          this.completeRows=response.data['data']['rows'];
          console.log('2222222',this.completeRows);
          // console.log('123'+this.indictmentRows[0]); // undefined
            // 将indictmentRows中picList中字符串转为字节串
            for (let i = 0; i < this.completeRows.length; i++) {
              const picList = this.completeRows[i].picList
              for (let j = 0; j < picList.length; j++) {
                this.completeRows[i].picList[j]='data:image/jpg;base64,' + this.completeRows[i].picList[j];
                // console.log('111'+this.indictmentRows[i].picList[j])
              }
            }
        }
      }catch(error) {
          // 处理错误
          this.$message({
              message: '错误' + error,
              type: 'error'
            });
        }
      },
      // 查询条件重置按钮
      handleReset() {
        this.searchForm.indictment_name = '';
        this.searchForm.person_name = '';
        this.searchForm.date = [];
        this.completeRows=[];
      },
      //对应表格中文件上传部分的处理
      beforeRemove(file) {
        return this.$confirm(`确定移除 ${ file.name }？`);
      },
      // 对上传的图片格式和大小进行验证
      onBeforeUpload(file)
      {
        const isIMAGE = file.type === 'image/jpeg'||'image/gif'||'image/png';
        const isLt1M = file.size / 1024 / 1024 < 1;

        if (!isIMAGE) {
          this.$message.error('上传文件只能是图片格式!');
        }
        if (!isLt1M) {
          this.$message.error('上传文件大小不能超过 1MB!');
        }        
        return isIMAGE && isLt1M;
      },
      // 点击修改按钮对应的处理
      changeInfoDialog(indictment) {
        this.changeIndictment={...indictment};
        this.changeInfoDialogVisible=true;
      },
      // 显示查看详情按钮点击操作
      showDetails(indictment) {
        this.showDetailsDialogVisible=true;
        this.showIndictment = {...indictment};
        console.log(this.showIndictment);
      },
      //处理表格显示分页的相关函数定义
      handlePageChange(currentPage) {
            this.currentPage=currentPage;
      },
      handlePageSizeChange(pageSize) {
        this.pageSize = pageSize;
        // 执行查询操作
        this.handleQuery();
      },
      // 图片识别结果展示函数
      pic_to_text(indictment) {
        indictment.status=1
        const ind = {...indictment}
        console.log(ind);
        const params={
          indictment_name:ind['indictment_name']
        }
        axios.post("/api/indictment/recognize_text", params).
        then(response =>{
          if (response.data['code']==0)
            this.recognize_indictment_texts={...response.data['data']['textList']}
            this.recognize_indictment_pics={...response.data['data']['picList']}
            // console.log("111111",this.recognize_indictment_pics);
            // console.log("2222222222",this.recognize_indictment_texts);
            // console.log(this.response.data['data']['picList'].length);
            console.log(response.data['data']['textList'].length);
            for (let i = 0; i < response.data['data']['textList'].length; i++){
              this.recognize_indictment_pics[i] = 'data:image/jpg;base64,' + this.recognize_indictment_pics[i];
              console.log("111111",this.recognize_indictment_pics[i]);
            }
            this.picInfoGetDialogVisible=true
        })
        .catch(error=>{
          console.log(error);
        }
        )
      }

    }
}
</script>

<style scoped>
.mySpan {
  color: black;
  font-weight: 450;
}
.myPic {
  width: 90%;
  height: 90%;
}
.pic-name-show {
  text-align: center;
  font-size: 30px; /* font size*/
  font-family: "SimHei", "黑体", sans-serif;
}
</style>