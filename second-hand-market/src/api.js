import axios from 'axios';

// 创建 axios 实例
const instance = axios.create({
  baseURL: '/api', // 使用相对路径，代理会将请求转发到后端
  timeout: 5000, // 请求超时设置
  headers: {
    'Content-Type': 'application/json', // 设置请求头
  },
});

instance.interceptors.request.use(
  config => {
    // 如果请求的数据是 FormData 类型，则删除 Content-Type 让浏览器自动处理
    if (config.data instanceof FormData) {
      delete config.headers['Content-Type'];  // 删除 'Content-Type'，让浏览器自动设置
    }
    return config;
  },
  error => {
    return Promise.reject(error);
  }
);
export default instance;
