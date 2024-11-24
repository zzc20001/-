<template>
  <div class="page">
    <!-- 左侧内容区 -->
    <div class="info-section">
      <h1>欢迎来到我们的平台</h1>
      <p>通过此平台，您可以迅速买卖二手物品。</p>
      <img src="https://via.placeholder.com/300x200" alt="装饰图案" class="decorative-img" />
    </div>

    <!-- 登录框 -->
    <div class="login-container">
      <h2>登录</h2>
      <form @submit.prevent="handleLogin">
        <div class="input-group">
          <label for="username">用户名</label>
          <input type="text" id="username" v-model="username" required placeholder="输入用户名" />
        </div>
        <div class="input-group">
          <label for="password">密码</label>
          <input type="password" id="password" v-model="password" required placeholder="输入密码" />
        </div>
        <button type="submit">登录</button>
      </form>
    </div>
  </div>
</template>


<script>  
import api from "../api";
export default {
  name: "UserLogin",
  data() {
    return {
      username: "",
      password: "",
    };
  },
  methods: {
    async handleLogin() {
  try {
    // 使用 API 发起登录请求
    const response = await api.post("/login", {
      username: this.username,
      password: this.password,
    });

    // 检查 HTTP 请求是否成功
    if (response.status === 200) {
      const data = response.data; // 后端返回的业务数据

      // 根据后端业务逻辑的状态码判断登录结果
      if (data.code === 200) { // 登录成功
        console.log("登录成功", data);
        const token = data.token; 
        if(token){
          sessionStorage.setItem("token", token);
        }
        alert("登录成功！欢迎：" + data.data); // 假设 data.data 是用户名称
        this.$router.push("/Mainpage"); // 登录成功后跳转页面
      } else if (data.code === 400) { // 登录失败，用户名或密码错误
        console.error("登录失败", data);
        alert("登录失败：用户名或密码错误！");
      } else { // 其他未知业务错误
        console.error("未知错误", data);
        alert("发生未知错误：" + data.message);
      }
    } else {
      console.error("HTTP 请求错误", response.status);
      alert("HTTP 请求失败，请稍后重试！");
    }
  } catch (error) {
    // 捕获请求失败的错误，例如网络问题
    console.error("请求失败", error);
    if (error.response) {
      // 检查后端返回的 HTTP 状态码
      if (error.response.status === 401) {
        alert("登录失败：用户名或密码不正确！");
      } else {
        alert("服务器错误，请稍后再试！状态码：" + error.response.status);
      }
    } else {
      // 网络错误或其他问题
      alert("请求失败，请检查网络连接！");
    }
  }
}
  },
};
</script>

<style scoped>/* 页面布局 */
.page {
  display: flex;
  align-items: center;
  justify-content: center;
  min-height: 100vh;
  margin: 0;
  padding: 0;
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  background: linear-gradient(145deg, #dcecf0, #f6f8fc); /* 柔和渐变 */
}

/* 左侧信息区 */
.info-section {
  flex: 1;
  max-width: 50%;
  padding: 2rem;
  text-align: center;
}

.info-section h1 {
  font-size: 2.5rem;
  color: #333;
  margin-bottom: 1rem;
}

.info-section p {
  font-size: 1.2rem;
  color: #666;
  margin-bottom: 2rem;
  line-height: 1.5;
}

.decorative-img {
  max-width: 100%;
  height: auto;
  border-radius: 10px;
  box-shadow: 0 8px 20px rgba(0, 0, 0, 0.1);
}

/* 登录容器 */
.login-container {
  flex: 1;
  max-width: 400px;
  margin: 2rem;
  padding: 2rem;
  background: white;
  border-radius: 15px;
  box-shadow: 0 8px 20px rgba(0, 0, 0, 0.1);
  text-align: center;
}

.login-container h2 {
  font-size: 2rem;
  color: #333;
  margin-bottom: 1.5rem;
}

/* 输入框样式 */
.input-group {
  margin-bottom: 1rem;
  text-align: left;
}

.input-group label {
  display: block;
  font-size: 0.9rem;
  margin-bottom: 0.5rem;
  color: #666;
}

input {
  width: 100%;
  padding: 12px;
  border: 1px solid #ddd;
  border-radius: 8px;
  background: #f9f9f9;
  font-size: 1rem;
  transition: border-color 0.3s, background-color 0.3s;
}

input:focus {
  border-color: #85b8cc;
  background-color: #fff;
  outline: none;
}

/* 按钮样式 */
button {
  width: 100%;
  padding: 14px;
  background-color: #74b9b7;
  border: none;
  border-radius: 8px;
  color: white;
  font-size: 1.2rem;
  font-weight: bold;
  cursor: pointer;
  transition: background-color 0.3s, transform 0.1s;
}

button:hover {
  background-color: #63a7a1;
}

button:active {
  transform: scale(0.98);
}

/* 响应式设计 */
@media (max-width: 768px) {
  .page {
    flex-direction: column;
  }

  .info-section {
    max-width: 100%;
    padding: 1rem;
  }

  .login-container {
    margin: 1rem;
    padding: 1.5rem;
  }
}
</style>