module.exports = {
  assetsDir: 'static',  // 指定静态资源文件夹
  devServer: {
    proxy: {
      '/api': {  // 将前端请求中的 /api 转发到后端服务器
        target: 'http://localhost:3000', // 这是后端的目标服务器地址
        changeOrigin: true, // 修改请求头中的 Origin 字段为目标 URL，避免 CORS 问题
        pathRewrite: {
          '^/api': '', // 将请求中的 /api 替换为空字符串，发送到后端的实际地址
        },
      },
    },
  },
};
