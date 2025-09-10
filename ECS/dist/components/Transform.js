"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Transform = void 0;
const Component_1 = require("../Component");
class Transform extends Component_1.Component {
    constructor(x = 0, y = 0, z = 0, rotation = 0, scale = 1) {
        super();
        this.x = x;
        this.y = y;
        this.z = z;
        this.rotation = rotation;
        this.scale = scale;
    }
    clone() {
        return new Transform(this.x, this.y, this.z, this.rotation, this.scale);
    }
}
exports.Transform = Transform;
//# sourceMappingURL=Transform.js.map