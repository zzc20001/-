<template>
  <div class="register-page">
    <div class="register-container">
      <h2>注册</h2>
      <form @submit.prevent="handleRegister">
        <div class="form-group">
          <label for="username">用户名:</label>
          <input type="text" id="username" v-model="username" required placeholder="请输入用户名" />
        </div>
        <div class="form-group">
          <label for="email">邮箱:</label>
          <input type="email" id="email" v-model="email" required placeholder="请输入邮箱" />
        </div>
        <div class="form-group">
          <label for="password">密码:</label>
          <input type="password" id="password" v-model="password" required placeholder="请输入密码" />
        </div>
        <button type="submit" class="submit-btn">注册</button>
      </form>
    </div>
  </div>
</template>

<script>
import api from "../api";

export default {
  name: "RegisterPage",
  data() {
    return {
      username: "",
      email: "", // 新增的邮箱字段
      password: "",
    };
  },
  methods: {
    async handleRegister() {
      try {
          const response = await api.post("/register", {
              username: this.username,
              email: this.email, // 新增的邮箱字段
              password: this.password,
          });
    
    // 根据响应的状态码判断是否成功
      if (response.status === 200) {
        console.log("注册成功", response.data);
        alert("注册成功！");
        this.$router.push("/login");  
      } else if (response.status === 400) {
        console.error("注册失败", response.data);
        alert("注册失败，请重试！");
      } else {
        console.error("未知错误", response.status);
        alert("发生未知错误，请稍后再试！");
      }
    } catch (error) {
      console.error("请求失败", error);
      alert("请求失败，请重试！");
      }
    }
  },
};
</script>

<style scoped>
.register-page {
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  background: linear-gradient(135deg, #fef6e4, #f9d7a5);
  background: url('../assets/高木.png') no-repeat center center/cover;
  font-family: Arial, sans-serif;
}

.register-container {
  width: 350px;
  padding: 2rem;
  background: #f9e5c6;
  border-radius: 15px;
  box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2), 0 4px 6px rgba(0, 0, 0, 0.1);
  display: flex;
  flex-direction: column;
  align-items: center;
}

h2 {
  font-size: 1.8rem;
  color: #7a4e34;
  margin-bottom: 2rem;
  font-weight: bold;
}

.form-group {
  width: 100%;
  margin-bottom: 20px;
  text-align: left;
}

.form-group label {
  font-size: 0.9rem;
  color: #7a4e34;
  margin-bottom: 0.5rem;
  display: inline-block;
}

input[type="text"],
input[type="email"], /* 新增邮箱样式 */
input[type="password"] {
  width: 100%;
  padding: 12px;
  font-size: 1rem;
  border: 1px solid #ffc78e;
  border-radius: 10px;
  background-color: #fff6e0;
  box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.05);
  transition: all 0.3s ease;
}

input:focus {
  border-color: #ff8a4c;
  outline: none;
  box-shadow: 0 0 5px rgba(255, 138, 76, 0.5);
}

.submit-btn {
  width: 100%;
  padding: 14px;
  font-size: 1.2rem;
  font-weight: bold;
  color: white;
  background: linear-gradient(135deg, #ff8a4c, #e6733f);
  border: none;
  border-radius: 12px;
  cursor: pointer;
  transition: all 0.3s ease;
  margin-top: 20px;
  box-shadow: 0 6px 12px rgba(0, 0, 0, 0.2);
}

.submit-btn:hover {
  background: linear-gradient(135deg, #ff8a4c, #d85c2d);
  box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3);
}

.submit-btn:active {
  background: linear-gradient(135deg, #ff8a4c, #c14e3f);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.25);
}

@media (max-width: 480px) {
  .register-container {
    width: 90%;
  }
}
</style>
