import axios from 'axios';

// 创建 axios 实例
const instance = axios.create({
  baseURL: '/api', // 使用相对路径，代理会将请求转发到后端
  timeout: 5000, // 请求超时设置
  headers: {
    'Content-Type': 'application/json', // 设置请求头
  },
});

export default instance;
