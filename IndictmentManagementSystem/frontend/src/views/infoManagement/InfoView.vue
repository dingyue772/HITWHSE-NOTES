<template>
    <div>
      <el-form :inline="true" :model="searchForm" class="demo-form-inline" style="border: 1px solid #eee">
          <el-form-item label="起诉书名称">
          <el-input v-model="searchForm.indictmentName"></el-input>
          </el-form-item>
          <el-form-item label="犯罪嫌疑人">
          <el-input v-model="searchForm.criminalName"></el-input>
          </el-form-item>
          <el-form-item label="涉嫌罪名">
          <el-input v-model="searchForm.crimeFacts"></el-input>
          </el-form-item>
          <el-form-item>
          <el-button type="primary" @click="handleSearch">查询</el-button>
          <el-button type="primary" @click="handleReset">重置</el-button>
          </el-form-item>
      </el-form>
      <!--将查询结果导出按钮-->
      <el-button type="info" style="float: right" @click="exportExcel('.responseTable', completeRows)">导出为excel表格</el-button>
      <!--显示查询结果的列表-->
      <el-table :data="criminalRows" border class="responseTable">
          <el-table-column prop="indictment_name" label="起诉书名称"></el-table-column>
          <el-table-column prop="criminal_name" label="嫌疑人姓名"></el-table-column>
          <el-table-column prop="race" label="民族"></el-table-column>
          <el-table-column prop="educational_level" label="文化程度"></el-table-column>
          <el-table-column prop="political_status" label="政治面貌"></el-table-column>
          <el-table-column prop="crime_facts" label="犯罪事实"></el-table-column>
          <el-table-column prop="id_card" label="身份证号码"></el-table-column>
          <el-table-column prop="home_place" label="户籍地"></el-table-column>
          <el-table-column prop="now_place" label="现居地"></el-table-column>
          <el-table-column prop="job_company" label="工作单位"></el-table-column>
      </el-table>
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
    </div>
</template>

<script>
import * as XLSX from 'xlsx'
import FileSaver from 'file-saver'
import axios from 'axios';
export default {
    data() {
        return {
            userName:sessionStorage.getItem('currentUser'),
            //criminalRows:[], // 用于显示犯罪嫌疑人数据的数组,是当前页的数据
            completeRows:[], // 从后端请求到的满足条件的全部数据
            searchForm:{
                indictmentName:'',
                criminalName:'',
                crimeFacts:''
            },
            //与分页有关的数据
            currentPage:1,
            pageSize:8, // 将每页展示记录数目固定为8
            total:0,
        };
       
    },
    computed:{
        criminalRows(){
            const start = (this.currentPage-1)*this.pageSize;
            const end = start+this.pageSize;
            return this.completeRows.slice(start, end);
        }
    },
    // 修改代码，get请求需要传递当前用户的用户名给后端来获得显示的criminalData，用户的用户名
    mounted() {
        // 从后端获取当前操作人上传起诉书的所有图片识别出来的嫌疑人的信息
        // this.fetchCriminalData();
    
    },
    methods: {
        // 获取嫌疑人信息请求函数
        // fetchCriminalData() {
        //     axios.get()
        // }
        async handleSearch() {
            try{
                const params = {
                    indictment_name:this.searchForm.indictmentName,
                    criminal_name:this.searchForm.criminalName,
                    crime_facts:this.searchForm.crimeFacts,
                    page:this.currentPage, // 当前页
                    per_page:this.pageSize // 每页的记录数
                }
                
                const response = await axios.post('api/criminal/query', params);
                if (response.data['code']==0){
                    // 处理响应结果
                    console.log('11111',response.data['data']);
                // 更新页面数据或执行其他操作
                this.total=response.data['data']['recordCount'];
                this.completeRows=response.data['data']['rows'];
                // console.log('2222222',this.criminalRows);
                // console.log(this.indictmentRows[0].picList[0].url); // undefined
                // 将indictmentRows中picList中字符串转为字节串
                }
            }catch(error){
                // 处理错误
            this.$message({
                message: '错误' + error,
                type: 'error'
                });
            }
        },
        handleReset() {
            // 处理重置逻辑
            console.log('执行重置操作');
            this.searchForm.indictmentName = '';
            this.searchForm.criminalName = '';
            this.searchForm.crimeFacts = '';
            this.criminalRows=[];
        },
        handlePageChange(currentPage) {
            this.currentPage=currentPage;
        },
        // 导出excel表格的处理
        exportExcel(className, data) {
            console.log('c:' + className)
            const fix = document.querySelector('.vxe-table--fixed-left-wrapper')
            let wb
            let ws
            if (fix) {
                ws = XLSX.utils.json_to_sheet(data, { header: Object.keys(data[0]) }) // Convert JSON data to worksheet
                wb = XLSX.utils.book_new() // Create a new workbook
                XLSX.utils.book_append_sheet(wb, ws, "Sheet1") // Add the worksheet to the workbook
                document.querySelector(className).appendChild(fix)
            } else {
                ws = XLSX.utils.json_to_sheet(data, { header: Object.keys(data[0]) }) // Convert JSON data to worksheet
                wb = XLSX.utils.book_new() // Create a new workbook
                XLSX.utils.book_append_sheet(wb, ws, "Sheet1") // Add the worksheet to the workbook
            }
            console.log(wb)
            const wbout = XLSX.write(wb, { bookType: 'xlsx', bookSST: true, type: 'array' })

            try {
                FileSaver.saveAs(new Blob([wbout], { type: 'application/octet-stream' }), '审核情况表.xlsx')
            } catch (e) {
                if (typeof console !== 'undefined') {
                    console.log(e, wbout)
                }
            }
            return wbout
        }
    }
}
</script >

<style>
</style>